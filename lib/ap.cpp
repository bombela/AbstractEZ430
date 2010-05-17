/*
 * ap.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/ap.h>

namespace ez430 {

class Implementation
{
	public:
		~Implementation()
		{
		}

		Implementation():
			_isOpen(false)
		{
		}

		Implementation(const std::string& line):
			_isOpen(false)
		{
			open(line);
		}

		void open(const std::string& line)
		{
			if (_isOpen)
				close();
		}

		bool isOpen() const
		{
			return false;
		}

		void close()
		{
		}

	private:
		bool _isOpen;
};

AccessPoint::~AccessPoint()
{
	delete _impl;
}

AccessPoint::AccessPoint():
	_impl(new Implementation) {}

AccessPoint::AccessPoint(const std::string& line):
	_impl(new Implementation(line)) {}

void AccessPoint::open(const std::string& line) { _impl->open(line); }
bool AccessPoint::isOpen() const { return _impl->isOpen(); }
void AccessPoint::close() { _impl->close(); }

} // namespace ez
