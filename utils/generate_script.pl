#!/usr/bin/perl -w
use strict;
use warnings;
use Curses::UI;
use YAML qw(LoadFile);
use Data::Dumper;
$Data::Dumper::Terse = 1;
use String::CamelCase qw(camelize decamelize wordsplit);
my $cui = new Curses::UI( -color_support => 1 );

sub select_type() {
	my $win            = $cui->add( 'win_types', 'Window' );
	my $selecting_type = 1;
	my $listbox        = $win->add(
		'type_list',
		'Listbox',
		-values => [ 'block', 'items', 'entity', 'window', 'craft' ],
		-title        => "Select type of script to generate    ",
		-titlereverse => 0,
		-labels       => {
			'block'  => 'Block script',
			'items'  => 'Item script',
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
		-oneline      => 1,
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
			$database_name_box->text( "block_" . decamelize( $this->{-text} ) );
			$class_name_box->text( "Block" . $this->{-text} . "Script" );
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
					my $script_name = $script_name_box->{-text};
					$selecting_methods = 0
					  if $script_name && $script_name =~ m/^[A-Z][a-zA-Z]*$/;
					$cui->error( -message => 'Script name is empty' )
					  if !$script_name;
					$cui->error(
						-message => 'Script name must be in CamelCase' )
					  unless $script_name =~ m/^[A-Z][a-zA-Z]*$/;
					$script_name_box->set_color_bfg('red');
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
	my @selected_methods = sort { $a <=> $b } $listbox->get();
	my $script_name      = $script_name_box->{-text};
	my $class_name       = $class_name_box->{-text};
	my $database_name    = $database_name_box->{-text};
	my $guard_name       = uc( decamelize($script_name) ) . "_H_";
	my $base_class       = "BlockScript";
	my $header_filename  = $class_name . ".h";
	my $code_filename    = $class_name . ".cpp";
	my $base_class_header = "Block/Scripts/BlockScript.h";

	# Create .h file
	open( HEADER_FILE, ">$header_filename" );
	print HEADER_FILE "#ifndef $guard_name\n";
	print HEADER_FILE "#define $guard_name\n\n";
    print HEADER_FILE "#include \"$base_class_header\"\n";

	print HEADER_FILE "\nnamespace Scripting\n{\n";
	print HEADER_FILE "\nclass $class_name: public $base_class\n{\n";
	print HEADER_FILE "public:\n    $class_name();";
	print HEADER_FILE "\n    virtual ~$class_name ();\n";
	for (@selected_methods) {
		print HEADER_FILE "    virtual "
		  . $methods_list_data->[$_]->{'code'} . ";\n";
	}
	printf HEADER_FILE ";}\n ";
	printf HEADER_FILE "\n} /* namespace Scripting */\n";
	printf HEADER_FILE "#endif /* $guard_name */\n";
	close(HEADER_FILE);

	# Create .cpp file
	open( CPP_FILE, ">$code_filename" );
	print CPP_FILE "#include $header_filename\n\n";
    print CPP_FILE "class $class_name"."::$class_name()\n    : $base_class(\"$script_name\")\n{\n}\n";
    print CPP_FILE "class $class_name"."::~$class_name()\n{\n}\n";
	for (@selected_methods) {
		my $code        = $methods_list_data->[$_]->{'code'};
		my @code_split  = split( ' ', $code );
		my $return_type = @code_split->[0];
		print CPP_FILE $return_type . " " . $class_name."::";
		shift @code_split;
		my $first = 0;
		for (@code_split) {
            print CPP_FILE " " if $first;
            $first = 1 unless $first;
			print CPP_FILE $_;
		}
		print CPP_FILE "\n{\n\n}\n";
	}
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
print "you selected: $selected_type\n";
