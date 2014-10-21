#ifndef CONNECTION_IMPL_HPP
#define CONNECTION_IMPL_HPP
#include <boost/smart_ptr/shared_ptr.hpp>

class ConnectionImpl
{
public:
	virtual std::string inputoutput(const std::string input)=0;
	virtual bool testconnection()=0;
	virtual void set_port()=0;
	virtual void reset()=0;
// 	connectionImpl(void);
// 	~connectionImpl(void);
protected:
	virtual bool start()=0;
	virtual void stop()=0;
// 	virtual int write(const std::string &buf)=0;
// 	virtual int write_async(const std::string &buf)=0;


	

};

template <class C> class ConnectionTemplate : public ConnectionImpl
{
public:
  C context;
};

template <class C> class Connection : public ConnectionTemplate<C>
{};
#endif