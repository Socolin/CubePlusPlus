#!/usr/bin/perl -w
use strict;
use warnings;
use File::Copy;
use File::Temp qw/ :mktemp  /;
use Curses::UI;
use YAML qw(LoadFile);
use String::CamelCase qw(camelize decamelize wordsplit);
my $cui = new Curses::UI( -color_support => 1 );

sub select_type() {
    my $win            = $cui->add( 'win_types', 'Window' );
    my $selecting_type = 1;
    my $listbox        = $win->add(
        'type_list',
        'Listbox',
        -values => [ 'block', 'item', 'entity', 'window', 'craft' ],
        -title        => "Select type of script to generate    ",
        -titlereverse => 0,
        -labels       => {
            'block'  => 'Block script',
            'item'  => 'Item script',
            'entity' => 'Entity script',
            'window' => 'Window script',
            'craft'  => 'Craft script',
        },
        -onchange => sub { $selecting_type = 0 },
        -border => 1,
    );
    $win->focus();
    $cui->focus( undef, 1 );    # 1 = forced focus
    $cui->draw();
    while ($selecting_type) { $cui->do_one_event }
    my $active_value = $listbox->get_active_value();
    $win->delete('type_list');
    $cui->delete('win_types');
    return $active_value;
}

sub select_methods {
    my $win               = $cui->add( 'win_methods', 'Window' );
    my $data              = LoadFile("generate_templates/$_[0]/data.yml");
    my $methods_data      = LoadFile("generate_templates/$_[0]/MethodList.yml");
    my $methods_list_data = $methods_data->{'methods'};
    my @methods           = ();
    my $methods_labesl;
    for ( my $i = 0 ; $i <= $#$methods_list_data ; $i++ ) {
        my $elt = $methods_list_data->[$i];
        $methods_labesl->{$i} = $elt->{'name'};
        push( @methods, $i );
    }
    my $selecting_methods = 1;
    my $infobox           = $win->add(
        'info_box',
        'TextViewer',
        -x            => 40,
        -height       => 15,
        -border       => 1,
        -title        => "Documentation",
        -titlereverse => 0,
        -wrapping     => 1,
        -vscrollbar   => 'right',
        -onfocus      => sub {
            my $this = shift;
            $this->{-bfg} = "blue";
            $this->{-sbg} = "blue";
        },
        -onblur => sub {
            my $this = shift;
            $this->{-bfg} = "white";
            $this->{-sbg} = "white";
        },
    );
    my $codebox = $win->add(
        'code_box',
        'TextViewer',
        -y            => 15,
        -height       => 1,
        -border       => 1,
        -title        => "Code",
        -titlereverse => 0,
        -onfocus      => sub {
            my $this = shift;
            $this->{-bfg} = "blue";
            $this->{-sbg} = "blue";
        },
        -onblur => sub {
            my $this = shift;
            $this->{-bfg} = "white";
            $this->{-sbg} = "white";
        },
    );
    my $listbox = $win->add(
        'list_methods',
        'Listbox',
        -values       => \@methods,
        -labels       => $methods_labesl,
        -border       => 1,
        -multi        => 1,
        -width        => 40,
        -title        => "Select methods used by the script",
        -titlereverse => 0,
        -height       => 15,
        -border       => 1,
        -vscrollbar   => 'right',
        -onfocus      => sub {
            my $this = shift;
            $this->{-bfg} = "blue";
            $this->{-sbg} = "blue";
        },
        -onblur => sub {
            my $this = shift;
            $this->{-bfg} = "white";
            $this->{-sbg} = "white";
        },
        -onselchange => sub {
            my $this = shift;
            $codebox->cursor_to_home();
            $infobox->text(
                $methods_list_data->[ $this->get_active_id() ]->{'doc'} );
            $codebox->text(
                $methods_list_data->[ $this->get_active_id() ]->{'code'} );
        },
    );
    my $class_name_box = $win->add(
        'class_name_box',
        'TextViewer',
        -oneline      => 1,
        -y            => 18,
        -x            => 50,
        -height       => 1,
        -border       => 1,
        -title        => "Class name",
        -titlereverse => 0,
        -focusable    => 0,
    );
    my $database_name_box = $win->add(
        'database_name_box',
        'TextViewer',
        -oneline      => 1,
        -y            => 18,
        -x            => 25,
        -height       => 1,
        -width        => 25,
        -border       => 1,
        -title        => "Database name",
        -titlereverse => 0,
        -focusable    => 0,
    );
    my $script_name_box = $win->add(
        'name_box',
        'TextEditor',
        -singleline   => 1,
        -y            => 18,
        -height       => 1,
        -width        => 25,
        -border       => 1,
        -title        => "Script name",
        -titlereverse => 0,
        -onfocus      => sub {
            my $this = shift;
            $this->{-bfg} = "blue";
            $this->{-sbg} = "blue";
        },
        -onblur => sub {
            my $this = shift;
            $this->{-bfg} = "white";
            $this->{-sbg} = "white";
        },
        -onchange => sub {
            my $this = shift;
            $database_name_box->text( $data->{'database_prefix'} . "_"
                  . decamelize( $this->{-text} ) );
            $class_name_box->text(
                $data->{'class_prefix'} . $this->{-text} . "Script" );
        }
    );
    $infobox->text(
        $methods_list_data->[ $listbox->get_active_id() ]->{'doc'} );
    $codebox->text(
        $methods_list_data->[ $listbox->get_active_id() ]->{'code'} );
    my $buttons = $win->add(
        'confirm_button',
        'Buttonbox',
        -y       => 21,
        -buttons => [
            {
                -label    => '< OK >',
                -shortcut => 'o',
                -value    => 1,
                -onpress  => sub {
                    # Checking if file already exist and ask to user if it
                    # must be ovewrite or not
                    my $script_name     = $script_name_box->{-text};
                    my $class_name      = $class_name_box->{-text};
                    my $header_filename = $class_name . ".h";
                    my $header_path     = $data->{'folder'} . $header_filename;
                    if ( -e $header_path ) {
                        my $return = $cui->dialog(
                            -message => "Overwrite $header_path ?",
                            -title   => "Overwrite",
                            -buttons => [ 'yes', 'no' ],
                        );
                        if ($return) {
                            $selecting_methods = 0;
                            return;
                        }
                    }
                    elsif ($script_name
                        && $script_name =~ m/^[A-Z][a-zA-Z]*$/ )
                    {
                        $selecting_methods = 0;
                        return;
                    }
                    $cui->error( -message => 'Script name is empty' )
                      if !$script_name;
                    $cui->error(
                        -message => 'Script name must be in CamelCase' )
                      unless $script_name =~ m/^[A-Z][a-zA-Z]*$/;
                    # Select the script name editor and mark it in red
                    my $this = shift;
                    $this->lose_focus();
                    $script_name_box->focus();
                    $script_name_box->set_color_bfg('red');
                  }
            },
            {
                -label    => '< Cancel >',
                -shortcut => 'c',
                -value    => 0,
                -onpress  => sub {
                    exit(0);
                  }
            }
        ]
    );

    # Define focus on method list
    $listbox->focus();
    $win->focus();
    $cui->focus( undef, 1 );    # 1 = forced focus
    $cui->draw();
    while ($selecting_methods) { $cui->do_one_event }

    # Save selected method
    my @selected_methods  = sort { $a <=> $b } $listbox->get();
    my $script_name       = $script_name_box->{-text};
    my $class_name        = $class_name_box->{-text};
    my $database_name     = $database_name_box->{-text};
    my $guard_name        = uc( decamelize($class_name) ) . "_H_";
    my $base_class        = $data->{'base_class'};
    my $header_filename   = $class_name . ".h";
    my $header_path       = $data->{'folder'} . $header_filename;
    my $code_path         = $data->{'folder'} . $class_name . ".cpp";
    my $base_class_header = $data->{'base_class_header'};

    # Variable to add lines in Register<Type>Script
    my $register_script_file  = $data->{'register_script_file'};
    my $register_line_include = "#include \"Basics/$header_filename\"\n";
    my $register_line_new     = "    new $class_name();\n";
    
    # Copy current file
    copy( $register_script_file, $register_script_file . ".old" );

    # Open backup
    open my $in,  '<', $register_script_file . ".old";
    # Open and truncate base file
    open my $out, '>', $register_script_file;

    # Loop over each line of file
    my $prev;
    while (<$in>) {

        # Insert line : `#include`, keeping alpha sort
        # Check if we are in `include` block to insert line
        if ( ( $_ =~ m/^#include "Basics/ )
            || ( $prev =~ m/^#include "Basics/ ) )
        {
            if ( ( !$prev || $prev lt $register_line_include )
                && ($_ eq "\n" || $_ gt $register_line_include ) )
            {
                print $out $register_line_include;
            }
        }

        # Insert line : `new Script();`, keeping alpha sort
        # Check if we are in `new` block to insert line
        if ( ( $_ =~ m/^    new / ) || ( $prev =~ m/^    new / ) ) {
            if ( ( !$prev || $prev eq "{\n" || $prev lt $register_line_new )
                && ($_ eq "\n" || $_ gt $register_line_new ) )
            {
                print $out $register_line_new;
            }
        }
        $prev = $_;
        print $out $_;
    }
    
    # Close files
    close $out;
    close $in;
    
    # Delete old file
    unlink $register_script_file . ".old";

    # Create .h file
    open( HEADER_FILE, ">$header_path" );

    # Guard
    print HEADER_FILE "#ifndef $guard_name\n";
    print HEADER_FILE "#define $guard_name\n\n";
    # Include base class header file
    print HEADER_FILE "#include \"$base_class_header\"\n";
    # Namespace
    print HEADER_FILE "\nnamespace Scripting\n{\n";
    # Define class
    print HEADER_FILE "\nclass $class_name: public $base_class\n{\n";
    # Constructor/Destructor
    print HEADER_FILE "public:\n    $class_name();";
    print HEADER_FILE "\n    virtual ~$class_name ();\n";
    # Add Copy method
    print HEADER_FILE "    virtual $base_class* Copy() override;\n";

    for (@selected_methods) {
        print HEADER_FILE "    virtual "
          . $methods_list_data->[$_]->{'code'} . " override;\n";
    }
    # End of class
    printf HEADER_FILE "};\n ";
    # End of namespace
    printf HEADER_FILE "\n} /* namespace Scripting */\n";
    # End of guard
    printf HEADER_FILE "#endif /* $guard_name */\n";
    close(HEADER_FILE);

    # Create .cpp file
    open( CPP_FILE, ">$code_path" );
    
    # Include header
    print CPP_FILE "#include \"$header_filename\"\n\n";
    # Define namespace
    print CPP_FILE "\nnamespace Scripting\n{\n\n";
    # Constructor
    print CPP_FILE "$class_name" . "::$class_name()";
    print CPP_FILE "\n    : $base_class(\"$script_name\")\n{\n}\n\n";
    # Destructor
    print CPP_FILE "$class_name" . "::~$class_name()\n{\n}\n\n";
    # Copy()
    print CPP_FILE "$base_class* $class_name" . "::Copy()\n{\n";
    print CPP_FILE "    return new $class_name(*this);\n}\n\n";

    for (@selected_methods) {
        my $code        = $methods_list_data->[$_]->{'code'};
        my @code_split  = split( ' ', $code );
        my $return_type = @code_split->[0];
        print CPP_FILE $return_type . " " . $class_name . "::";
        shift @code_split;
        my $first = 0;
        for (@code_split) {
            print CPP_FILE " " if $first;
            $first = 1 unless $first;
            print CPP_FILE $_;
        }
        print CPP_FILE "\n{\n\n}\n\n";
    }
    # End of namespace
    printf CPP_FILE "\n} /* namespace Scripting */\n";
    close(CPP_FILE);

    # Cleaning windows
    $win->delete('info_box');
    $win->delete('code_box');
    $win->delete('list_methods');
    $win->delete('confirm_button');
    $cui->delete('win_methods');
    return @selected_methods;
}
my $selected_type    = select_type();
my @selected_methods = select_methods($selected_type);
$cui->leave_curses();
