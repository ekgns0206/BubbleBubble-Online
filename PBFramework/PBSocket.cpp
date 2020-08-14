#include "PBSocket.h"
using namespace PB;

WSAData socket::_wsa;

socket::socket(int af, int type, int protocol)
{
	this->_socket			= ::socket(af, type, protocol);
	if (this->_socket == INVALID_SOCKET)
		throw std::exception("socket error");
}

socket::socket(SOCKET sock)
{
	this->_socket			= sock;
}

socket::~socket()
{
}

//
// bind
//	������ ���ε��Ѵ�.
//
// Parameters
//	port		��Ʈ
//
// Return
//	������ true, ���н� false
//
bool socket::bind(short port)
{
	struct sockaddr_in		addr	= {0,};
	addr.sin_family					= AF_INET;
	addr.sin_port					= htons(port);
	addr.sin_addr.s_addr			= INADDR_ANY;

	return ::bind(this->_socket, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

//
// close
//	������ �����Ѵ�.
// 
// Parameters
//	����
//
// Return
//	����
// 
void socket::close()
{
	closesocket(this->_socket);
}

socket::operator SOCKET() const
{
	return this->_socket;
}

//
// init
//	������ ����ϱ� ���� �ʱ�ȭ�� �����Ѵ�.
//
// Parameters
//	����
//
// Return
//	����
//
void socket::init()
{
	WSAStartup(MAKEWORD(2, 2), &_wsa);
}

//
// release
//	���� �ʱ�ȭ�� ���� �޸𸮸� �����Ѵ�.
//
// Parameters
//	����
//
// Return
//	����
//
void socket::release()
{
	WSACleanup();
}

tcp::tcp() : socket(AF_INET, SOCK_STREAM, 0)
{
}

tcp::tcp(SOCKET sock) : socket(sock)
{
}

tcp::~tcp()
{
}

//
// listen
//	Ŭ���̾�Ʈ�� ���� �غ� �Ѵ�.
//
// Parameters
//	����
//
// Return
//	������ true, ���н� false
//
bool tcp::listen()
{
	return ::listen(this->_socket, SOMAXCONN) != SOCKET_ERROR;
}

//
// accept
//	���� ��û�� Ŭ���̾�Ʈ�� �޴´�.
//
// Parameters
//	����
//
// Return
//	���� ��û�� Ŭ���̾�Ʈ ����
//
tcp tcp::accept()
{
	struct sockaddr_in	addr		= {0,};
	int					addrsize	= sizeof(addr);
	SOCKET				client		= ::accept(this->_socket, (struct sockaddr*)&addr, &addrsize);

	if (client == SOCKET_ERROR)
		throw std::exception("accept error");

	return tcp(client);
}

//
// connect
//	���� ��û�� �Ѵ�.
//
// Parameters
//	ip		������
//	port	��Ʈ
//
// Return
//	������ true, ���н� false
//
bool tcp::connect(const std::string& ip, short port)
{
	struct sockaddr_in	addr		= {0,};
	addr.sin_family					= AF_INET;
	addr.sin_addr.s_addr			= inet_addr(ip.c_str());
	addr.sin_port					= htons(port);

	if (addr.sin_addr.s_addr == -1)
	{
		struct hostent* entry = gethostbyname(ip.c_str());
		if (entry == NULL)
			return false;

		addr.sin_addr.s_addr = *((unsigned long *)(entry->h_addr_list[0]));
	}

	return ::connect(this->_socket, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

//
// send
//	�����͸� ������.
//
// Parameters
//	bytes		���� ������
//
// Return
//	������ true, ���н� false
//
bool tcp::send(const std::vector<char>& bytes)
{
	int					result		= ::send(this->_socket, bytes.data(), bytes.size(), 0);
	if (result == SOCKET_ERROR)
		return false;

	return true;
}

//
// recv
//	�����͸� �޴´�.
//
// Parameters
//	buffer		����� ����
//	size		���� ũ��
//
// Return
//	������ true, ���н� false
//
bool tcp::recv(std::vector<char>& buffer, int size)
{
	char				temp[256]	= {0,};
	int					recvsize	= 0;
	int					amount		= 0;

	buffer.clear();
	while(size > amount)
	{
		recvsize		= ::recv(this->_socket, temp, min(sizeof(temp), size - amount), 0);
		if(recvsize == SOCKET_ERROR)
			return false;

		if(recvsize == 0)
			return false;

		buffer.insert(buffer.end(), temp, temp + recvsize);
		amount			+= recvsize;
	}
	
	return true;
}

udp::udp() : socket(AF_INET, SOCK_DGRAM, 0)
{
}

udp::~udp()
{
}

bool udp::sendto(const std::string & ip, short port, const std::vector<char>& bytes) throw(std::exception)
{
	struct sockaddr_in	addr			= {0,};
	addr.sin_family						= AF_INET;
	addr.sin_addr.s_addr				= inet_addr(ip.c_str());
	addr.sin_port						= htons(port);

	int					sendsize		= ::sendto(this->_socket, bytes.data(), bytes.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
	if(sendsize == SOCKET_ERROR)
		return false;

	return true;
}

bool udp::recvfrom(std::string & ip, short * port, std::vector<char>& buffer) throw(std::exception)
{
	struct sockaddr_in	addr			= {0,};
	int					addrsize		= sizeof(addr);

	buffer.resize(4096);
	buffer.clear();
	int					recvsize		= ::recvfrom(this->_socket, buffer.data(), buffer.size(), 0, (struct sockaddr*)&addr, &addrsize);
	if(recvsize == SOCKET_ERROR)
		return false;

	buffer.resize(recvsize);
	return true;
}

socket_map::socket_map()
{}

socket_map::~socket_map()
{
	this->clear();
}

bool socket_map::add(tcp* socket)
{
	SOCKET fd = (SOCKET)*socket;
	if(this->find(fd) != this->end())
		return false;

	const std::pair<SOCKET, tcp*> pair(fd, socket);
	this->insert(pair);
	return true;
}

bool socket_map::remove(tcp* socket)
{
	SOCKET fd = (SOCKET)*socket;
	if(this->find(fd) == this->end())
		return false;

	this->erase(fd);
	return true;
}

void socket_map::clear()
{
	for(std::map<SOCKET, tcp*>::iterator i = this->begin(); i != this->end(); i++)
		delete i->second;

	std::map<SOCKET, tcp*>::clear();
}

socket_map::iterator socket_map::begin()
{
	return __super::begin();
}

socket_map::iterator socket_map::end()
{
	return __super::end();
}

socket_map::citerator socket_map::begin() const
{
	return __super::begin();
}

socket_map::citerator socket_map::end() const
{
	return __super::end();
}

socket_map::operator const fd_set & () const
{
	fd_set					ret = { 0, };

	for(std::map<SOCKET, tcp*>::const_iterator i = this->begin(); i != this->end(); i++)
		FD_SET((SOCKET)*i->second, &ret);

	return ret;
}

client::client(const std::string & host, short port) : _thread(client::onThreadRoutine, this)
{
	this->_host				= host;
	this->_port				= port;
}

client::~client()
{
	this->_thread.wait();
}

bool PB::client::connect()
{
	if(tcp::connect(this->_host, this->_port) == false)
		return false;

	this->onConnected(*this);
	this->_thread.run();
	return true;
}

//
// onThreadRoutine
//	�����忡�� ������ �Լ��̴�.
//	recv �Լ��� �̿��� �����͸� �޴� �۾��� �Ѵ�.
//	�����͸� ���������� �޴´ٸ� onReceive �Լ��� ȣ���Ѵ�.
//
// Parameters
//	thread		������
//
// Return
//	����
//
void client::onThreadRoutine(thread * thread)
{
	client*						self		= (client*)thread->parameter();
	bool						disconnect	= false;

	while(self->onReceive(*self))
	{ }

	self->onDisconnect(*self);
	self->close();
}

server::server(int port) : tcp(), _thread(this->onThreadRoutine, this)
{
	this->_running = false;
	this->bind(port);
	this->listen();
}

server::~server()
{
	this->close();
}

bool server::run()
{
	if(this->_running)
		return false;

	this->_running = true;
	this->_thread.run();
	return true;
}

bool server::close()
{
	if(this->_running == false)
		return false;

	this->_running = false;
	this->_thread.wait();
	socket::close();

	for (auto i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		this->_clients.remove(i->second);
		delete i->second;
	}
	this->_clients.clear();
	return true;
}

socket_map* server::clients()
{
	return &(this->_clients);
}

void server::onThreadRoutine(thread* t)
{
	server*					self		= (server*)t->parameter();
	SOCKET					fd			= (SOCKET)*self;
	std::vector<tcp*>		disconnectedList;

	while(self->_running)
	{
		fd_set					cpyReads	= self->_clients;
		struct timeval			timeout		= { 5, 0 };

		FD_SET(fd, &cpyReads);
		int						fdnum		= select(0, &cpyReads, 0, 0, &timeout);
		if(fdnum == SOCKET_ERROR)								// error
			break;

		if(fdnum == 0)											// timeout
			continue;

		if(FD_ISSET((SOCKET)*self, &cpyReads))					// connection request
		{
			tcp			socket		= self->accept();
			self->_clients.add(self->onConnected(socket));
			puts("connected");
		}

		disconnectedList.clear();
		for(socket_map::iterator i = self->_clients.begin(); i != self->_clients.end(); i++)
		{
			tcp*		client		= i->second;
			SOCKET		fd			= (SOCKET)*client;
			if(FD_ISSET(fd, &cpyReads) == false)
				continue;

			if(self->onReceive(*client) == false)
			{
				puts("disconnected");
				self->onDisconnected(*client);
				disconnectedList.push_back(client);
			}
		}

		for(std::vector<tcp*>::iterator i = disconnectedList.begin(); i != disconnectedList.end(); i++)
		{
			self->_clients.remove(*i);
			delete *i;
		}
	}
}