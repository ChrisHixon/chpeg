#!/usr/bin/ruby

require_relative 'top_down'

module CH::TopDown::Peg

    class NodeAllocator
        def initialize(mod, default_node_class)
            @mod = mod
            @default_node_class = default_node_class
        end
        def allocate(params=nil, options=nil)
            id = params[:id]
            begin
                klass = @mod.const_get(params[:id])
            rescue
                klass = @default_node_class
            end
            klass.new(params, options)
        end
    end

    module Nodes
        class Node < CH::TopDown::Node
            attr_reader :node_id, :rule_name
            attr_accessor :peg_options
            def setup(params = nil)
                setup_begin # good place to initialize subclass items
                setup_params(params)
                setup_pre_children # good place to finalize initialization, after params / @vars setup
                setup_children
                setup_finalize # see caution below
            end
            def setup_begin
            end
            def setup_params(params=nil)
                params = params ? params.dup : {}
                params.each do |k, v|
                    instance_variable_set("@#{k}", v)
                end
            end
            def setup_pre_children
                @root ||= self
            end
            def setup_children
                if @data.kind_of?(Array)
                    @data.each do |c|
                        c.setup({:parent => self, :root => @root })
                    end
                end
            end
            def setup_finalize
                # caution: this is run after children setup(), but before parent's setup_finalize(),
                # due to it being called inside setup()
                # this limits its usefulness; you may want to consider post_setup, post_reduce
            end
            def post_setup
                if @data.kind_of? Array
                    @data.each {|c| c.post_setup }
                end
            end
            def reduce
                if @data.kind_of? Array
                    new_data = []
                    @data.each do |c|
                        res = c.reduce
                        new_data << res if res
                    end
                    @data = new_data
                end
                self
            end
            def post_reduce
                @node_id = @root.add_node(self)
                if @data.kind_of? Array
                    @data.each {|c| c.post_reduce }
                end
            end
            def rule_setup(rule_name = nil, depth = 0)
                @rule_name, @depth = rule_name, depth
                if @data.kind_of?(Array)
                    @data.each_with_index do |c,i|
                        c.rule_setup(rule_name && "#{@rule_name}[#{i}]", depth+1)
                    end
                end
            end
            def tree_format(level, str, txt="")
                "// %5d %5d %20s %4s | %s %s%s" % [
                    @offset, @length, @id.inspect, (@node_id ? @node_id.to_i : '-'),
                    "  "*level + str,
                    @rule_name ? "<#{@rule_name}> " : "",
                    @peg_options ? "{#{@peg_options}}" : "" ]
            end
            def to_s
                "<:#{id} \##{@node_id}>"
            end
            def inspect
                "<#{@depth?"-"*@depth:""}:#{id} #{@node_id} (#{rule_name}) #{@data.kind_of?(Array) ? "[]" : value.inspect}>#{@peg_options&&" {#{@peg_options}}"}"
            end
        end

        # create classes for each node type with superclass of our Node
        [ :Grammar, :Definition, :Choice, :Sequence,
            :Predicate, :Repeat, :Primary, :Identifier, :Literal,
            :CharClass, :CharRange, :Char, :EscChar, :OctChar, :Dot
        ].each do |n|
            module_eval <<-END
                class #{n} < Node
                end
            END
        end

        class Definition
            def post_setup
                @peg_options ||= ""
                if @data[1].id == :options
                    @peg_options += @data[1].value
                    @data.delete_at(1)
                end
                @peg_options += "I" if @data[1].id == :Predicate
                super
            end
            def rule_setup(rule_name, depth)
                @rule_name, @depth = "def:#{@data[0].value}", depth
                @data[0].rule_setup("id:#{@data[0].value}", depth+1)
                @data[1].rule_setup(@data[0].value, depth+1)
            end
        end

        class Grammar
            def setup_begin
                @nodes = []
                super
            end
            def rule_setup(rule_name=nil, depth=0)
                super
                @rule_name = "Grammar"
            end
            def add_node(node)
                id = @nodes.length
                @nodes << node
                id
            end
        end

        class Choice
            def post_setup
                @data.each do |c|
                    c.peg_options ||= ""
                    c.peg_options += "E"
                end
                super
            end
            #def reduce
            #    # convert a choice of 1-length chars to CharClass
            #    cc = [ :CharClass, :CharRange, :Char, :EscChar, :OctChar, :Literal ]
            #    if @data.all? {|c| cc.index(c.id) && c.value.length == 1 }
            #        nd = @data.collect {|c| c.value }.join("")
            #        return @rule.parser.allocator.allocate({
            #            :id => :CharClass, :offset => @offset, :length => @length,
            #            :data => @data, :parent => @parent, :root => @root,
            #            :rule => @rule.parser.rules[:CharClass]
            #        })
            #    end
            #    super
            #end
        end

        class Sequence
        end

        class Repeat
            def post_setup
                @op = @data.pop.value
                if @data[0].id == :Sequence
                    @data[0].peg_options ||= ""
                    @data[0].peg_options += "E"
                end
                super
            end
        end

        class Predicate
            def post_setup
                @op = @data.shift.value
                super
            end
        end

        class Literal
            def post_setup
                @value = @matches[1].gsub("\\n", "\n").
                    gsub("\\r", "\r").gsub("\\t", "\t").gsub("\\\\","\\")
            end
        end

        class CharRange
            def post_setup
                super
                @value = "#{@data[0].value}-#{@data[1].value}"
            end
        end

        class EscChar
            def post_setup
                @value = @data.gsub("\\n", "\n").
                    gsub("\\r", "\r").
                    gsub("\\t", "\t").
                    gsub(/\\(.)/, '\1')
                super
            end
        end

        class OctChar
            def post_setup
                @value = @data.gsub(/\\([0-3][0-7][0-7]|[0-7][0-7]?)/) { $1.to_i(8).chr }
                super
            end
        end
    end # modules Nodes
end # module CH::TopDown::Peg


module CH::TopDown::Peg

# optimized and transformed to generate the desired parse tree
# note: don't autoformat/indent this section
PegGrammar = {
    # Hierarchical syntax
    :Grammar => [ :wsc, [ :Definition, '+'], :eof ],
    :Definition => [ :Identifier, [:options, '?'], :leftarrow, :Choice ],
    :Choice => [ :Sequence, [ :slash, :Sequence, '*'], { :unwrap => true }  ],
    :Sequence => [ [ [ :Predicate, '+' ] ], { :unwrap => true } ],
    :Predicate => [ [ [ :and, '|', :not ], '?'], :Repeat, { :unwrap => true } ],
    :Repeat => [ :primary, [ [ :question, '|', :star, '|', :plus ], '?'], { :unwrap => true } ],
    :primary => [ :Identifier, [ '!', [:options, '?'], :leftarrow ], { :unwrap => true },
        '|', :open, :Choice, :close, { :unwrap => true },
        '|', :Literal , '|', :CharClass , '|', :Dot
    ],
    # Lexical syntax
    :options => [ /\{\s*([ILUSE]*)\s*\}/, :wsc, { :rewrite => true } ],
    :Identifier => [ :identifier, :wsc, { :rewrite => true } ],
    :Literal => [ :literal1, :wsc, { :rewrite => true },
        '|', :literal2, :wsc, { :rewrite => true } ],
    :CharClass => [ :lsquare, :CharRange, [ [ [ '!', /\]/ ], :CharRange, { :unwrap => true } ], '*'],
        :rsquare, :wsc ],
    :CharRange => [ :Char, [/-/, { :ignore => true } ], [ '!', /\]/ ], :Char, '|', :Char ], # corrected adding !']'
    :Char => [ :EscChar, '|', :OctChar, '|', /[^\\]/ ],
    :EscChar => /\\[nrt'"\[\]\\]/,
    :OctChar => /\\([0-3][0-7][0-7]|[0-7][0-7]?)/, # corrected (presumably) to 0-3

    :lsquare =>     [ /\[/, { :ignore => true } ],
    :rsquare =>     [ /\]/, { :ignore => true } ],
    :leftarrow =>   [ /<-/, :wsc, { :ignore => true } ],
    :slash =>       [ /\//, :wsc, { :ignore => true } ],
    :and =>         [ /&/,  :wsc, { :rewrite => true } ],
    :not =>         [ /!/,  :wsc, { :rewrite => true } ],
    :question =>    [ /\?/, :wsc, { :rewrite => true } ],
    :star =>        [ /\*/, :wsc, { :rewrite => true } ],
    :plus =>        [ /\+/, :wsc, { :rewrite => true } ],
    :open =>        [ /\(/, :wsc, { :ignore => true } ],
    :close =>       [ /\)/, :wsc, { :ignore => true } ],
    :Dot =>         [ /\./, :wsc, { :rewrite => true } ],

    :literal1 => /'([^']*)'/, # technically wrong, should fix TODO (needs to use :Char)
    :literal2 => /"([^"]*)"/, # technically wrong, should fix TODO (needs to use :Char)
    :identifier => /[a-zA-Z_][a-zA-Z_0-9]*/,
    :wsc => [ /(?:\s+|#\s*([^\r\n]*)[\r\n]*)*/, { :ignore => true } ], # whitespace or comment
    :eof => [ /$/, { :ignore => true } ],
}

end
