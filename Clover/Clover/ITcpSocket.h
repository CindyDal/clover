#ifndef ITCPSOCKET_H_
#define ITCPSOCKET_H_

#include <string>

class ITcpSocket
{
public:
	virtual ~ITcpSocket() {}
	virtual void SendMessage(std::string message) = 0;
	virtual void Initialize() = 0;
	virtual void Create(std::string serverAdress,std::string serverPort) = 0;
	virtual void Create(std::string port) = 0;
	virtual void Bind() = 0;
	virtual void Listen() = 0;
	virtual void Accept() = 0;
	virtual void ConnectToServer() = 0;
	virtual void Close() = 0;
	virtual void CloseClient() = 0;
	virtual std::string ReceiveMessage() = 0;
};

#endif // !ITCPSOCKET_H_