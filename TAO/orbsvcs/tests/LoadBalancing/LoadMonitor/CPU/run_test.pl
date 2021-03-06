eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$location = 'MyLocation';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorfile = "lm.ior";

#Files which used by server1
my $server1_iorfile = $server1->LocalFile ($iorfile);
$server1->DeleteFile($iorfile);

#Files which used by server2
my $server2_iorfile = $server2->LocalFile ($iorfile);
$server2->DeleteFile($iorfile);

#Files which used by server2
my $client_iorfile = $client->LocalFile ($iorfile);
$client->DeleteFile($iorfile);


$SV1 = $server1->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/LoadBalancer/tao_loadmanager",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server1_iorfile");

$SV2 = $server2->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/LoadBalancer/tao_loadmonitor",
                              "-ORBdebuglevel $debug_level " .
                              "-l $location " .
                              "-t CPU -s PULL " .
                              "-ORBInitRef LoadManager=file://$server2_iorfile");

$CL = $client->CreateProcess ("client",
                              "-l $location " .
                              "-ORBInitRef LoadManager=file://$client_iorfile");

print STDERR "\n\n==== Running CPU Load Monitor test\n";

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($iorfile,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server1->GetFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server2->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$server2_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV2->TerminateWaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server_status = $SV1->TerminateWaitKill ($server1->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server1->DeleteFile($iorfile);
$server2->DeleteFile($iorfile);
$client->DeleteFile($iorfile);

exit $status;
