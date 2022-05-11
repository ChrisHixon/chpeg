require_relative 'peg'


module CH::TopDown::Peg
    module CByte
        BootstrapDir = File.dirname(__FILE__)
        class OutStream
            Indent = "    "
            attr_accessor :indent
            def initialize(filename)
                @file = File.open(filename, "w")
                @indent = 0
            end
            def puts(str)
                str.each_line do |line|
                    line.chomp!
                    @file.write("#{Indent * @indent}#{line}\n")
                end
            end
            def close
                @file.close
            end
        end

        class Output
            attr_reader :c, :h, :basename
            def initialize(basename)
                @basename = basename
                @c = OutStream.new("#{basename}.c")
                @h = OutStream.new("#{basename}.h")
            end
            def close
                @c.close
                @h.close
            end
        end

        class ParserGenerator
            attr_reader :dir, :basename, :prefix, :node, :out
            def initialize(dir, basename, prefix, node)
                @dir = dir
                @basename = basename
                @prefix = prefix
                @node = node
                @out = nil
            end
            def output_parser()
                @out = Output.new(@dir + '/' + @basename)
                @node.parse_setup
                @out.c.puts("#include <string.h>")
                @out.c.puts("#include <stdio.h>")
                @out.c.puts("#include <stdlib.h>")
                @out.c.puts("#include \"#{basename}.h\"")
                @out.c.puts("\n")
                Nodes::Classes.each do |c|
                    klass = Nodes.const_get(c)
                    klass.output_class_begin(self)
                end
                node.output(self)
                Nodes::Classes.each do |c|
                    klass = Nodes.const_get(c)
                    klass.output_class_end(self)
                end
                @out.close
                @out = nil
            end
        end

        module Nodes

            module NodeClassExtension
                def output_class_begin(pg)
                end
                def output_class_end(pg)
                end
            end

            module NodeExtension
                attr_reader :parse_state
                def parse_setup
                    @parse_state = @root.alloc_state(self)
                    if @data.kind_of? Array
                        @data.each {|c| c.parse_setup}
                        @success_state = @root.alloc_state(self)
                        @failure_state = @root.alloc_state(self)
                    end
                end
                def fail_state(child)
                    @failure_state
                end
                def output_parse(out)
                    out.c.puts "// ***** #{self.inspect}"
                    if @data.kind_of? Array
                        @data.each {|c| c.output_parse(out)}
                    end
                end
                def add_inst(inst, inst_num, opcode, arg) # helper func
                    raise "instruction ordering error: inst.length(#{inst.length}) != inst_num(#{inst_num})" if inst.length != inst_num
                    inst << "/* %5d %-40s */ INST(#{opcode}, #{arg})" % [ inst_num, self.inspect ]
                end
                def append_instructions(inst)
                    if @data.kind_of? Array
                        @data.each {|c| c.append_instructions(inst)}
                    end
                end
                def flags
                    return "0" unless @peg_options
                    f = { "S" => "STOP", "I" => "IGNORE", "L" => "LEAF" }
                    out = []
                    @peg_options.each_byte {|c| out << f[[c].pack("C")]}
                    out.length > 0 ? out.join(" | ") : "0"
                end
            end

            Classes = [
                :Node, :Grammar, :Definition, :Choice, :Sequence,
                :Predicate, :Repeat, :Identifier, :Literal,
                :CharClass, :CharRange, :Char, :EscChar, :OctChar, :Dot
            ]

            SubClasses = { }

            Classes.each do |k|
                module_eval <<-END
                    class #{k} < CH::TopDown::Peg::Nodes::#{SubClasses[k] || k}
                        include NodeExtension
                    end
                END
                const_get(k).class_eval { extend NodeClassExtension }
            end

            # delete this if not used
            module Container
            end

            ContainerClasses = [ ]

            ContainerClasses.each do |k|
                const_get(k).class_eval { include Container }
            end

            class Grammar
                def self.output_class_begin(pg)
                    File.open(File.join(BootstrapDir, "peg_cbyte.h"), "r") do |file|
                        file.each_line do |line|
                            pg.out.h.puts(line)
                        end
                    end
                end
                def self.output_class_end(pg)
                    File.open(File.join(BootstrapDir, "peg_cbyte.c"), "r") do |file|
                        file.each_line do |line|
                            pg.out.c.puts(line)
                        end
                    end
                end
                def post_setup
                    @states = []
                    @strings = []
                    @str_hash = {}
                    @defs = []
                    @def_hash = {}
                    super
                end
                def add_string(str)
                    return @str_hash[str] if @str_hash[str]
                    str_id = @strings.length
                    @strings << str
                    @str_hash[str] = str_id
                    str_id
                end
                def add_def(node)
                    def_id = @defs.length
                    @defs << node
                    @def_hash[node.data[0].value] = node
                    def_id
                end
                def resolve_ident(id)
                    @def_hash[id]
                end
                def alloc_state(node)
                    ret_id = @states.length
                    @states << node
                    ret_id
                end
                def parse_setup
                    @parse_state = @root.alloc_state(self)
                    @failure_state = @root.alloc_state(self)
                    @success_state = @root.alloc_state(self)
                    if @data.kind_of? Array
                        @data.each {|c| c.parse_setup}
                    end
                end
                def append_instructions(inst)
                    add_inst(inst, @parse_state, :IDENT, 0)
                    add_inst(inst, @failure_state, :FAIL, 0)
                    add_inst(inst, @success_state, :SUCC, 0)
                    super
                end
                def output(pg)
                    len = @defs.length
                    pre = pg.prefix
                    pre_up = pre.upcase
                    out = pg.out
                    out.h.puts @defs.zip((0..@defs.length).to_a).collect{|d,i| "#define #{pre_up}_#{d.data[0].value.upcase} #{i}"}.join("\n")
                    out.c.puts "\n"
                    out.c.puts "int #{pre}_num_defs = #{len};"
                    out.c.puts "char *#{pre}_def_names[] = {#{@defs.collect{|d| d.data[0].value.inspect}.join(", ")}};"
                    out.c.puts "int #{pre}_def_flags[] = {#{@defs.collect{|d| d.flags}.join(", ")}};"
                    out.c.puts "int #{pre}_def_addrs[] = {#{@defs.collect{|d| "#{d.parse_state-1}"}.join(", ")}}; // presubtracted by 1"
                    out.c.puts "\nint #{pre}_num_strings = #{@strings.length};"
                    out.c.puts "char *#{pre}_strings[] = {#{@strings.collect{|s| s.inspect}.join(", ")}};"
                    out.c.puts "int #{pre}_str_len[] = {#{@strings.collect{|s| s.length}.join(", ")}};"
                    inst = []
                    append_instructions(inst)
                    out.c.puts "\nint #{pre}_num_instructions = #{inst.length};"
                    out.c.puts "int #{pre}_instructions[] = {"
                    out.c.indent += 1
                    out.c.puts inst.join(",\n")
                    out.c.indent -= 1
                    out.c.puts "};"
                    out.c.puts "ByteCode #{pre}_byte_code = {"
                    out.c.indent += 1
                    out.c.puts "#{len},"
                    out.c.puts "#{pre}_def_names,"
                    out.c.puts "#{pre}_def_flags,"
                    out.c.puts "#{pre}_def_addrs,"
                    out.c.puts "#{inst.length},"
                    out.c.puts "#{pre}_instructions,"
                    out.c.puts "#{@strings.length},"
                    out.c.puts "(unsigned char **)#{pre}_strings,"
                    out.c.puts "#{pre}_str_len,"
                    out.c.indent -= 1
                    out.c.puts "};"
                    out.h.puts "extern ByteCode #{pre}_byte_code;"
                    out.h.puts "\n#endif //PEG_CBYTE_PARSER_H\n"
                end
            end

            class Definition
                attr_reader :def_id
                def next_state(child)
                    raise "Child not found: #{child}" unless @data[1] == child
                    @success_state
                end
                def post_reduce
                    @def_id = @root.add_def(self)
                    super
                end
                def parse_setup
                    @data[1].parse_setup
                    @success_state = @root.alloc_state(self)
                    @failure_state = @root.alloc_state(self)
                end
                def append_instructions(inst)
                    @data[1].append_instructions(inst)
                    add_inst(inst, @success_state, :ISUCC, def_id)
                    add_inst(inst, @failure_state, :IFAIL, 0)
                end
                def parse_state
                    @data[1].parse_state
                end
            end

            class Identifier
                def parse_setup
                    @parse_state = @root.alloc_state(self)
                    @goto_state = @root.alloc_state(self)
                end
                def append_instructions(inst)
                    def_id = @root.resolve_ident(value).def_id
                    add_inst(inst, @parse_state, :IDENT, def_id)
                    add_inst(inst, @goto_state, :GOTO, @parent.fail_state(self) - 1)
                end
            end

            class Choice # note: a simpler implementation might create an intermediate parent for each child
                def parse_setup
                    @parse_state = @root.alloc_state(self)
                    @child_succ_states = {}
                    @child_fail_states = {}
                    if @data.kind_of? Array
                        @data.each do |c|
                            c.parse_setup
                            @child_succ_states[c] = @root.alloc_state(self)
                            @child_fail_states[c] = @root.alloc_state(self)
                        end
                    end
                    @failure_state = @root.alloc_state(self)
                end
                def next_state(child)
                    @child_succ_states[child]
                end
                def fail_state(child)
                    @child_fail_states[child]
                end
                def append_instructions(inst)
                    add_inst(inst, @parse_state, :CHOICE, 0)
                    @data.each do |c|
                        c.append_instructions(inst)
                        add_inst(inst, @child_succ_states[c], :CISUCC, @parent.next_state(self) - 1)
                        add_inst(inst, @child_fail_states[c], :CIFAIL, 0)
                    end
                    add_inst(inst, @failure_state, :CFAIL, @parent.fail_state(self) - 1)
                end
                def output_parse(out)
                    out.c.puts "\n// BEGIN #{self.inspect}"
                    out.c.puts "case #{@parse_state}:"
                    out.c.indent += 1
                    out.c.puts "if (top >= self->max_stack_size - 3) return -5;"
                    out.c.puts "stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point"
                    out.c.puts "stack[++top] = offset; // save offset for backtrack"
                    out.c.indent -= 1

                    @data.each do |c|
                        c.output_parse(out)

                        out.c.puts "\n// CSUCC #{c.inspect}"
                        out.c.puts "case #{@child_succ_states[c]}:"
                        out.c.indent += 1
                        out.c.puts "top -= 2;"
                        out.c.puts "state = #{@parent.next_state(self)}; continue;"
                        out.c.indent -= 1

                        out.c.puts "\n// CFAIL #{c.inspect}"
                        out.c.puts "case #{@child_fail_states[c]}:"
                        out.c.indent += 1
                        out.c.puts "offset = stack[top]; // restore saved offset (backtrack)"
                        out.c.puts "for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i) " +
                            "{ Node_pop_child(tree_stack[tree_top]); }"
                        out.c.puts "// pass through"
                        out.c.indent -= 1
                    end

                    out.c.puts "\n// FAIL  #{self.inspect}"
                    out.c.puts "case #{@failure_state}:"
                    out.c.indent += 1
                    out.c.puts "top -= 2;"
                    out.c.puts "state = #{@parent.fail_state(self)}; continue;"
                    out.c.indent -= 1

                    out.c.puts "\n// END   #{self.inspect}"
                end
            end

            class Sequence
                def parse_setup
                    @data.each {|c| c.parse_setup}
                end
                def next_state(child)
                    idx = @data.index(child)
                    raise "Child not found: #{child}" unless idx
                    @data[idx + 1] ? @data[idx + 1].parse_state : @parent.next_state(self)
                end
                def fail_state(child)
                    @parent.fail_state(self)
                end
                def parse_state
                    @data[0].parse_state
                end
            end


            class Repeat # TODO: the cnt stack var is only needed for op +
                def next_state(child)
                    raise "Child not found: #{child}" unless @data[0] == child
                    @success_state
                end
                def append_instructions(inst)
                    case @op
                    when '+'
                        add_inst(inst, @parse_state, :RPBEG, 0)
                        super
                        add_inst(inst, @success_state, :RPMAT, @data[0].parse_state - 1)
                        add_inst(inst, @failure_state, :RPDONE, @parent.fail_state(self) - 1)
                    when '*'
                        add_inst(inst, @parse_state, :RSBEG, 0)
                        super
                        add_inst(inst, @success_state, :RSMAT, @data[0].parse_state - 1)
                        add_inst(inst, @failure_state, :RSDONE, 0)
                    when '?'
                        add_inst(inst, @parse_state, :RQBEG, 0)
                        super
                        add_inst(inst, @success_state, :RQMAT, 0)
                        add_inst(inst, @failure_state, :RQDONE, 0)
                    end
                end
            end

            class Predicate
                def next_state(child)
                    raise "Child not found: #{child}" unless @data[0] == child
                    @success_state
                end
                def match_inst
                    @op == "&" ? :PMATCHS : :PMATCHF
                end
                def nomat_inst
                    @op == "&" ? :PNOMATF : :PNOMATS
                end
                def append_instructions(inst)
                    add_inst(inst, @parse_state, :PRED, 0)
                    super
                    add_inst(inst, @success_state, match_inst, @parent.fail_state(self) - 1)
                    add_inst(inst, @failure_state, nomat_inst, @parent.fail_state(self) - 1)
                end
            end

            class CharClass
                def post_reduce
                    @value = @data.collect{|c| c.value}.join
                    @str_id = @root.add_string(@value)
                end
                def parse_setup
                    @parse_state = @root.alloc_state(self)
                    @goto_state = @root.alloc_state(self)
                end
                def append_instructions(inst)
                    add_inst(inst, @parse_state, :CHRCLS, @str_id)
                    add_inst(inst, @goto_state, :GOTO, @parent.fail_state(self) - 1)
                end
            end

            class Literal
                def post_reduce
                    @str_id = @root.add_string(value)
                end
                def parse_setup
                    @parse_state = @root.alloc_state(self)
                    @goto_state = @root.alloc_state(self)
                end
                def append_instructions(inst)
                    add_inst(inst, @parse_state, :LIT, @str_id)
                    add_inst(inst, @goto_state, :GOTO, @parent.fail_state(self) - 1)
                end
            end

            class Dot
                def parse_setup
                    @parse_state = @root.alloc_state(self)
                end
                def append_instructions(inst)
                    add_inst(inst, @parse_state, :DOT, @parent.fail_state(self) - 1)
                end
            end

            # CharRange, Char, EscChar, and OctChar are never alone; they're always leaf nodes of CharClass,

            class CharRange
            end

            class Char
            end

            class EscChar
            end

            class OctChar
            end

        end # module Nodes

    end # module CByte

end # module CH::TopDown::Peg
