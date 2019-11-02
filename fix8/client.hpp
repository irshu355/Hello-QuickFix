#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "globals.hpp"

class client_session;

/// Example client message router. Derives from fix8 generated router class.
/*! Your application must define a class similar to this in order to receive
    the appropriate callback when Message::process is called. */

class tex_router_client : public FIX8::TEX::Texfix_Router
{

    client_session &_session;

public:
    /*! Ctor.
	        \param session client session */
    tex_router_client(client_session &_session) : _session(_session) {}

    /*! Execution report handler. Here is where you provide your own methods for the messages you wish to
		 handle. Only those messages that are of interest to you need to be implemented.
	    \param msg Execution report message session */

    virtual bool operator()(const FIX8::TEX::ExecutionReport *msg) const;
};

class client_session : public FIX8::Session
{
    tex_router_client _router;

public:
    /*! Ctor. Initiator.
	    \param ctx reference to generated metadata
	    \param sid sessionid of connecting session
		 \param persist persister for this session
		 \param logger logger for this session
		 \param plogger protocol logger for this session */

    client_session(const FIX8::F8MetaCntx &ctx, const FIX8::SessionID &sid, FIX8::Persister *persist = nullptr,
                   FIX8::Logger *logger = nullptr, FIX8::Logger *plogger = nullptr) : Session(ctx, sid, persist, logger, plogger), _router(*this) {}

    /*! Application message callback.
	    This method is called by the framework when an application message has been received and decoded. You
	  	 should implement this method and call the supplied Message::process.
	    \param seqnum Fix sequence number of the message
		 \param msg Mesage decoded (base ptr)
		 \return true on success */
    bool handle_application(const unsigned seqnum, const FIX8::Message *&msg);

    /*! This method id called whenever a session state change occurs
	    \param before previous session state
	    \param after new session state */
    void state_change(const FIX8::States::SessionStates before, const FIX8::States::SessionStates after);
};

//-----------------------------------------------------------------------------------------
/// A random number generator wrapper.
struct RandDev
{
    static void init()
    {
        time_t tval(time(0));
#ifdef _MSC_VER
        srand(static_cast<unsigned>(((tval % _getpid()) * tval)));
#else
        srandom(static_cast<unsigned>(((tval % getpid()) * tval)));
#endif
    }

    template <typename T>
    static T getrandom(const T range = 0)
    {
#ifdef _MSC_VER
        T target(rand());
#else
        T target(random());
#endif
        return range ? target / (RAND_MAX / range + 1) : target;
    }
};

#endif