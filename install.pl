#!/usr/bin/perl

# *******************************************************************
# Program: generator.pl
# Author : Piotr Pszczolkowski (piotr@beesoft.org)
# Date   : 2006-03-18
# *******************************************************************

use strict;
use Cwd;

my $bindir       = "/usr/bin";
my $resdir       = "/usr/share/bsc";
my $langdir      = "/usr/share/bsc/lang";
my $homedir      = "/home/" . getlogin() . "/bsc";
my $dst_bin_dir  = "";
my $dst_res_dir  = "";
my $dst_lang_dir = "";
my @bin_files    = ( "bsc" );
my @res_files    = ( "BeesoftCommander.png", "licence.txt", "help.en.html" );
# my @lang_files   = ( "bsc_pl.qm", "bsc_de.qm", "bsc_ru.qm", "bsc_es.qm", "bsc_cs.qm" );
my $make_prg     = "make";
my $qmake_prg    = "qmake";
my $strip_prg    = "strip";
my $err_msg      = "Error: can't start a ";
my $endl2        = "\n\n";
my $ok           = "OK" . $endl2;


print "\n";
print "==========================================\n";
print "                                          \n";
print "       Beesoft Commander Generator        \n";
print "                                          \n";
print "==========================================\n";
print "\n";

print "------- QMAKE ----------------------------\n";
die $err_msg . $qmake_prg . $endl2 if system( $qmake_prg );
print $ok;

print "------- MAKE CLEAN -----------------------\n";
die $err_msg . $make_prg . $endl2 if system( $make_prg, "clean" );
system( "rm", "-f", "bsc" );
print $ok;

print "------- MAKE -----------------------------\n";
die $err_msg . $make_prg . $endl2 if system( $make_prg );
print $ok;

print "------- STRIP ----------------------------\n";
die $err_msg . $make_prg . $endl2 if system( $strip_prg, "bsc" );
print $ok;

####### COPY DATA TO DESTINATION LOCATIONS ###########################

my $can_copy = 0;

if( make_dir( $resdir )  ) {
   if( make_dir( $langdir ) ) {
      $dst_bin_dir  = $bindir;
      $dst_res_dir  = $resdir;
 #     $dst_lang_dir = $langdir;
      $can_copy = 1;
    }
}
elsif( make_dir( $homedir ) ) {
   $dst_bin_dir  = $homedir;
   $dst_res_dir  = $homedir;
#   $dst_lang_dir = $homedir;
   $can_copy = 1;
}
else {
   print "ERROR: you don't have any permissions.\n";
}

print "------- COPYING ---------------------------\n";
if( $can_copy ) {
   copy_files( $dst_bin_dir , @bin_files );
   copy_files( $dst_res_dir , @res_files );
#   copy_files( $dst_lang_dir, @lang_files );

   my $prg_path = $dst_bin_dir . "/bsc";
   chmod( 0755, $prg_path );
   
   print $ok . ". Please wait.\n";
   system( $prg_path );
}


#*********************************************************************
#  make_dir
#*********************************************************************
sub make_dir
{
   my $dir_name    = $_[0];
   my $current_dir = cwd();
   my $retval      = 0;

   if( chdir $dir_name ) {
      if( make_bak( $dir_name ) ) {
         $retval = 1;
      }
   }
   elsif ( mkdir $dir_name ) {
      $retval = 1;
   }

   chdir $current_dir;
   return $retval;
}

#*********************************************************************
# make_bak
#*********************************************************************
sub make_bak
{
   my $dirname = $_[0];
   my $retval = 0;

   if( -d $dirname && -w $dirname && -x $dirname ) {
      if( opendir RESDIR, $dirname ) {
         my @files = readdir RESDIR;
         foreach( @files ) {
            if( -f  && -w ) {
               my $new_fname = $_ . '.old';
               rename( $_, $new_fname );
            }
         }
         $retval = 1;
      }   
   }
}

#*********************************************************************
# copy_files
#*********************************************************************
sub copy_files
{
   my $src_dir = cwd();
   my $dst_dir;
   my @files;
   ( $dst_dir, @files ) = @_;

   my $cmd;
   my $fname;
   foreach $fname ( @files ) {
      my $src_path = $src_dir . "/" . $fname;
      my $dst_path = $dst_dir . "/" . $fname;
      print ">>>  $src_path ---> $dst_path" . "\n";
      system( "cp " . $src_path . " " . $dst_path );
   }
}
