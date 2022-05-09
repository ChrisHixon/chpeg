#
# Copyright (C) 2009-2015 Chris Hixon                       vim: noai:ts=2:sw=2
# All rights reserved
#
# Simple grammar rule based top-down recursive-descent parser
#
# I discovered after writing this that it's PEG but with regexp terminals
# http://en.wikipedia.org/wiki/Parsing_expression_grammar
# After discovering PEG, I modified it to include all PEG concepts
#
# Notes:
# - be careful with Choices, they always resolve to one of the choices and
#   can't be given options except in the sub-choice lists (options on a
#   Choice as a whole are ignored/lost).
# - if you don't want something to be resolved/replaced, try putting [ ] around the rule
#   (this may work to give a choice options as well)
#
# TODO:
#
# - think about collapse/unwrap option naming - isn't it more of an expand/splat?
# - separate text data and child node data in Nodes
#   if this is done, should text be the text the node represents?
#   if so, probably easiest to get it from input[offset, len], might even be
#   a method w/cache, since text may not even be used except leaf nodes
#   (reading this later i'm not sure i even follow)
# - empty repeats and possibly other stuff can lead to infinite loop which
#   isn't detected by current stuff. need to add a time limit (threads??)
#   *did the time limit thing but maybe think about checking for problems too
#
# FEATURE ADDITIONS
# - execute code from rules after match or other points (using existing extend)
# - detect line number and char in line where error occurs [ done, sort of, when printing error ]
#
# OTHER
# - tests
# - optimize, after other changes stabilize
# - document grammar, options
# - make into a gem and submit it to a repo?

require 'strscan'

module CH
module TopDown

class Node
  attr_reader :id, :offset, :length, :data, :rule, :options, :matches
  def initialize(params=nil, options=nil)
    (params || {}).each do |k, v|
      instance_variable_set("@#{k}", v)
    end
    raise "rule not provided" unless @rule
    @options = @rule.options.merge(options || {})
    extend(@options[:extend]) if @options[:extend]
  end
  def ignore?
    @options[:ignore]
  end
  def collapse?
    @options[:collapse] ||
      (@options[:unwrap] && @data.kind_of?(Array) &&
        @data.length == 1) ||
      (@options[:collapse_len] && @data.kind_of?(Array) &&
        @data.length == @options[:collapse_len])
  end
  def value
    if @value
      @value
    elsif !@options[:no_capture] && @matches && @matches.length > 1
      @matches[@options[:capture_value] || 1]
    else
      @data
    end
  end
  def inspect
    "{%s %d %d %s}" % [@id.inspect, @offset, @length, value.inspect ]
  end
  def tree_format(level, str, txt="")
    "%5d %5d %20s | %s" % [
      @offset, @length, @id.inspect, "  "*level + str ]
  end
  def print(level=0)
    if @data.kind_of?(String)
      val = respond_to?(:value) ? value.inspect : "'#{@data}'"
      puts tree_format(level, "#{@id.inspect} #{val}")
    elsif @data.kind_of?(Array)
      puts tree_format(level, "#{@id.inspect} [")
      @data.each do |d|
        d.print(level+1)
      end
      puts tree_format(level, "]")
    else
      puts tree_format(level, "#{@id.inspect} #{@data.inspect}")
    end
  end
  def check_data(input, display=false, level=0)
    # check that the input is captured properly with offset/start
    inp = input[@offset, @length]
    check_data_print(inp) if display
    if @data.kind_of?(String)
      if inp != @data
        puts "F: in='#{inp}' out='#{@data}' offset=#{@offset} length=#{@length}"
      end
    elsif @data.kind_of?(Array)
      @data.each do |d|
        d.check_data(input, display, level+1)
      end
    end
  end
  def check_data_print(inp)
    puts "%3d %3d %16s| '%s'" % [@offset, @length, @id.inspect, inp]
  end
end

class StringScanner < ::StringScanner
  def get_expected_data
    if defined? @__expected_max
      { :rule => @__expected_rule, :item => @__expected_item,
        :position => @__expected_max, :expected_match => @__expected_match,
        :msg => expected_message }
    else
      nil
    end
  end
  def expected_message
    l1 = []; l2 = []

    item_summary = @__expected_item.options[:summary]
    item_id = @__expected_item.id.inspect
    if item_summary
      l1 << "#{item_summary}"
      l2 << "item #{item_id}"
    else
      l1 << "item #{item_id}"
    end

    if @__expected_rule
      rule_summary = @__expected_rule.options[:summary]
      rule_id = @__expected_rule.id.inspect
      if rule_summary
        l1 << "in #{rule_summary}"
        l2 << "rule #{rule_id}"
      else
        l1 << "in rule #{rule_id}"
      end
    end

    "#{@__expected_match ? "expected" : "unexpected"} #{l1.join(" ")}\n" <<
      "(#{l2.join(", ")})"
  end
  def expected(rule, item, position, expected_match=true)
    @__expected_max ||= 0
    if position >= @__expected_max
      @__expected_rule = rule
      @__expected_item = item
      @__expected_max = position
      @__expected_match = expected_match
      #puts "#{position} #{rule ? rule.id.inspect : 'nil'} #{item.id.inspect} #{expected_match}"
    end
  end
end

class Allocator
  def allocate(params=nil, options=nil)
    Node.new(params, options)
  end
end

class Parser
  attr_reader :error_pos, :error_msg, :rules, :allocator
  def initialize(grammar, allocator=nil)
    @grammar = grammar;
    @allocator = allocator || Allocator.new;
    setup_rules
  end
  def print_error_old
    input, pos, msg = @input.string, @error_pos, @error_msg
    start = pos - 35; start = 0 if start < 0
    endp = pos + 35; endp = input.length if endp > input.length
    caret = pos - start
    puts "Error at or near offset #{pos}: #{msg}"
    puts input[start, 1+endp-start]
    puts " "*caret + "^"
  end
  def print_error
    input, pos, msg = @input.string, @error_pos, @error_msg
    nl = /\r\n|\n|\r|$/
    row, col, last_pos, line = 0, 0, 0, ""
    @input.pos = 0
    while line = @input.scan_until(nl) # too bad StringScanner doesn't count lines...
      break if @input.pos >= pos
      last_pos = @input.pos
      row += 1
    end
    col = pos - last_pos
    puts "Error at or near offset #{pos} (line #{row+1}, column #{col+1}): #{msg}"
    puts "> #{line}"
    puts "  " + " "*(col) + "^"
  end
  def parse(input, start_rule, timeout=nil, consume_all=true)
    t = Thread.new {
      @error_pos, @error_msg, res = nil, nil, nil
      @input = StringScanner.new(input)
      res = @rules[start_rule].parse(@input, @allocator)
      if !res
        if (expected = @input.get_expected_data)
          @error_pos = expected[:position]
          @error_msg = expected[:msg]
        else
          @error_pos = @input.pos
          @error_msg = "No match."
        end
      elsif consume_all && res && !@input.eos?
        if (expected = @input.get_expected_data)
          @error_pos = expected[:position]
          @error_msg = expected[:msg]
        else
          @error_pos = @input.pos
          @error_msg = "Unexpected input."
        end
        p res
        res = nil
      #else
        #while res.collapse? and res.data.length == 1
        #  res = res.data[0]
        #end
      end
      res
    }
    raise "Exceeded time limit" unless t.join(timeout)
    t.value
  end

  def setup_rules
    @rules = {}
    @grammar.each do |id, rule_def|
      # simulate an alias (:rule1 => :rule2) using a Choice
      rule_def = [ rule_def, '|' ] if rule_def.kind_of? Symbol
      @rules[id] = Rule.create(id, rule_def, self)
    end
    @rules.each { |k,v| v.resolve(@rules) }
  end

  class Rule
    attr_reader :id, :rule_def, :options, :parser
    def self.create(id, rule_def, parser, options=nil)
      if rule_def.kind_of?(Regexp)
        Terminal.new(id || rule_def, rule_def, parser, options)
      elsif rule_def.kind_of?(Array)
        ItemList.create(id, rule_def, parser, options)
      elsif rule_def.kind_of?(Symbol)
        Placeholder.new(id, rule_def, parser, options)
      else
        raise "Invalid rule #{id.inspect}: => #{rule_def.inspect}"
      end
    end
  end

  class Placeholder < Rule
    def initialize(id, sym, parser, options=nil)
      @parser = parser
      @id, @sym, @options = id, sym, options
      raise "options given to Placeholder: #{inspect}" if options
    end
    def resolve(rules)
      rules[@sym] or raise "rule not found for Placeholder: #{inspect}"
    end
    def inspect
      ":(id=#{@id.inspect}, sym=#{@sym.inspect}, options=#{@options.inspect})"
    end
  end

  class ItemList < Rule
    attr_reader :items
    def self.create(id, rule_def, parser, options=nil)
      # the order of if/elseif here is important
      if rule_def.index("|")
        Choice.new(id, rule_def, parser, options)
      elsif rule_def.last.kind_of?(Hash)
        options = rule_def.last
        rule_def = rule_def[0...-1]
        Rule.create(id, rule_def.length > 1 ? rule_def : rule_def[0], parser, options)
      elsif rule_def.first.kind_of?(String) && rule_def.first =~ /^[&!]$/
        Predicate.new(id, rule_def, parser, options)
      elsif rule_def.last.kind_of?(String) && rule_def.last =~ /^([*?+]|\{.*\})$/
        Repeat.new(id, rule_def, parser, options)
      else
        Sequence.new(id, rule_def, parser, options)
      end
    end
    def resolve(rules)
      @items.each_index { |i| @items[i] = @items[i].resolve(rules) }
      self
    end
  end

  class Terminal < Rule
    attr_reader :re
    def initialize(id, regexp, parser, options=nil)
      @parser = parser
      @options = options || {}
      @id, @regexp, @rule_def = id, regexp, regexp
    end
    def parse(input, allocator)
      offset = input.pos
      s = input.scan(@regexp)
      return nil unless s
      matches, i = [], 0
      while true
          m = input[i]
          break unless m
          matches << m
          i += 1
      end
      allocator.allocate({ :id => @id, :offset => offset, :length => s.length,
        :data => s, :matches => matches, :rule => self })
    end
    def resolve(rules)
      self
    end
  end

  class Sequence < ItemList
    def initialize(id, sequence_def, parser, options=nil)
      @parser = parser
      @options = options || {}
      @id, @rule_def, @items = id, sequence_def, []
      sequence_def.each_with_index do |item, i|
        @items << Rule.create("#{@id}[#{i}]", item, parser)
      end
    end
    def parse(input, allocator)
      start, out, matches = input.pos, [], nil
      @items.each do |item|
        pos = input.pos
        res = item.parse(input, allocator)
        if res && !res.ignore?
          if res.collapse?
            out += res.data
          else
            out << res
          end
        end
        unless res
          input.expected(self, item, pos)
          return nil
        end
      end
      if out.length == 1 && @options[:rewrite]
        start = out[0].offset
        len = out[0].length
        matches = out[0].matches
        out = out[0].data
      else
        len = input.pos - start
      end
      return allocator.allocate({ :id => @id, :offset => start, :length => len,
        :data => out, :matches => matches, :rule => self })
    end
  end

  class Predicate < Rule
    attr_reader :operator
    def initialize(id, rule_def, parser, options=nil)
      @parser = parser
      @options = { :ignore => true }.merge(options || {})
      @id, @rule_def = id, rule_def
      @operator = rule_def.shift
      raise "Empty predicate rule #{id.inspect}" if rule_def.empty?
      unless @operator == '&' || @operator == '!'
        raise "Unknown operator: #{@operator}"
      end
      @item = Rule.create("#{@id}[0]", rule_def, parser)
    end
    def resolve(rules)
      @item = @item.resolve(rules)
      self
    end
    def parse(input, allocator)
      start = input.pos
      res = @item.parse(input, allocator)
      if @operator == '&' && !res
        input.expected(self, @item, start, true)
        input.pos = start # don't consume
        return nil
      elsif @operator == '!' && res
        input.expected(self, @item, start, false)
        input.pos = start # don't consume
        return nil
      end
      len = input.pos - start
      input.pos = start # don't consume
      return allocator.allocate({ :id => @id, :offset => start, :length => len,
                                :data => nil, :matches => nil, :rule => self })
    end
  end

  class Repeat < ItemList
    attr_reader :op, :min, :max
    def initialize(id, repeat_def, parser, options=nil)
      @parser = parser
      @options = { :collapse => true }.merge(options || {})
      @id, @rule_def, @items, m, n = id, repeat_def, [], nil, nil
      @op = repeat_def.last; repeat_def = repeat_def[0...-1]
      raise "Empty repeat/optional rule #{id.inspect}" if repeat_def.empty?
      case @op # these ops work like the equiv. in regexps
      when '*'; m=0; n=nil # * matches zero or more occurrences
      when '+'; m=1; n=nil # + matches one or more occurrences
      when '?'; m=0; n=1   # ? matches zero or one occurrences
      # {m,n} = at least m, at most n
      when /^\{(0|[1-9][0-9]*),(0|[1-9][0-9]*)\}$/; m=$1.to_i; n=$2.to_i
      when /^\{(0|[1-9][0-9]*),\}$/; m=$1.to_i; n=nil # {m,} at least m
      when /^\{(0|[1-9][0-9]*)\}$/; m=$1.to_i; n=m # {m} exactly m
      else; raise "Invalid repeat/optional operation: #{@op.inspect}"
      end
      @min, @max = m, n
      repeat_def.each_with_index do |item, i|
        @items << Rule.create("#{@id}[#{i}]", item, parser)
      end
    end
    def parse(input, allocator)
      start, cnt, res, out = input.pos, 0, nil, []
      reached, expected_item, matches = 0, nil, nil
      begin
        backtrack = input.pos; seq = []
        success = @items.all? do |item|
          reached = input.pos
          expected_item = item
          res = item.parse(input, allocator)
          if res && !res.ignore?
            if res.collapse?
              seq += res.data
            else
              seq << res
            end
          end
          res
        end
        if success
          cnt += 1; out += seq
        else
          input.pos = backtrack
        end
      end until !res || (@max && cnt >= @max)
      if (cnt >= @min) && (!@max || cnt <= @max)
        options = {}
        options[:ignore] = (cnt==0) if @options[:ignore].nil?
        if out.length == 1 && @options[:rewrite]
          start = out[0].offset
          len = out[0].length
          matches = out[0].matches
          out = out[0].data
        else
          len = input.pos - start
        end
        allocator.allocate({ :id => @id, :offset => start, :length => len,
          :data => out, :matches => matches, :rule => self }, options)
      else
        input.expected(self, expected_item, reached) if !expected_item.nil?
        nil
      end
    end
  end

  class Choice < ItemList
    def initialize(id, choice_def, parser, options=nil)
      @parser = parser
      @options = options || {}
      @id, @rule_def, @items, cur, choices = id, choice_def, [], [], []
      choice_def.each do |item|
        if item == '|'
          choices << (cur.length > 1 ? cur : cur[0]) unless cur.empty?
          cur = []
        else
          cur << item
        end
      end
      choices << (cur.length > 1 ? cur : cur[0]) unless cur.empty?
      choices.each do |item|
        @items << Rule.create(@id, item, parser)
      end
    end
    def parse(input, allocator)
      backtrack = input.pos
      @items.each do |item|
        res = item.parse(input, allocator)
        return res if res
        input.pos = backtrack
      end
      input.expected(nil, self, backtrack)
      nil
    end
  end

end # class Parser

end # module TopDown
end # module CH
