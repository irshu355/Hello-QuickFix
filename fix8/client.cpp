#include <iostream>
#include <memory>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <typeinfo>
#include <thread>
#ifdef _MSC_VER
#include <signal.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <signal.h>
#include <termios.h>
#endif

#include <errno.h>
#include <string.h>

/* f8 headers */

#include <fix8/f8includes.hpp>

#ifdef FIX8_HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <fix8/usage.hpp>
#include <fix8/consolemenu.hpp>
#include <fix8/multisession.hpp>

#include "fix/Texfix_types.hpp"
#include "fix/Texfix_router.hpp"
#include "fix/Texfix_classes.hpp"

#include "client.hpp"

using namespace std;
using namespace FIX8;

void client_process(ClientSessionBase *mc);
void print_usage();
const string GETARGLIST("hl:svqc:R:S:rdomN:D:");
f8_atomic<bool> term_received(false);

bool quiet(false);
unsigned next_send(0), next_recieve(0);

void sig_handler(int sig)
{
    switch (sig)
    {
    case SIGTERM:
    case SIGINT:
#ifndef _MSC_VER
    case SIGQUIT:
#endif
        term_received = true;
        signal(sig, sig_handler);
        break;
    }
}

int main(int argc, char **argv)
{

    int val;
    bool server(false), reliable(false), once(false), dump(false), multi(false);
    string clcf, session;

    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
#ifndef _MSC_VER
    signal(SIGQUIT, sig_handler);
#endif

    bool restore_tty(false);

    const string config_file("client_config.xml");

    f8_atomic<unsigned> scnt(0);

    const string my_session(session.empty() ? "DLD1" : session);

    unique_ptr<ClientSessionBase> mc(new ClientSession<client_session>(TEX::ctx(), config_file, my_session));

    //unique_ptr<ClientSessionBase> mc(new ReliableClientSession<client_session>(TEX::ctx(), config_file, my_session));

    mc->session_ptr()->control() |= Session::printnohb;

    mc->start(false, next_send, next_recieve, mc->session_ptr()->get_login_parameters()._davi());

    client_process(mc.get());

    return 0;
}

//clang++ -std=c++11 client.cpp -lPocoFoundation -lPocoNet -lPocoUtil -lPocoXML -lfix8 -lpthread -lz -g -O2 -L/usr/local -lPocoNetd -lmyfix

void client_process(ClientSessionBase *mc)
{
    if (!mc->session_ptr()->is_shutdown())
        mc->session_ptr()->stop();
}

//-----------------------------------------------------------------------------------------
bool client_session::handle_application(const unsigned seqnum, const Message *&msg)
{
    return enforce(seqnum, msg) || msg->process(_router);
}

//-----------------------------------------------------------------------------------------
void client_session::state_change(const FIX8::States::SessionStates before, const FIX8::States::SessionStates after)
{
    cout << get_session_state_string(before) << " => " << get_session_state_string(after) << endl;
}

bool tex_router_client::operator()(const TEX::ExecutionReport *msg) const
{
    TEX::LastCapacity lastCap;
    if (msg->get(lastCap))
    {
        // if we have set a realm range for LastCapacity, when can check it here
        if (!quiet && !lastCap.is_valid())
            cout << "TEX::LastCapacity(" << lastCap << ") is not a valid value" << endl;
    }
    return true;
}
