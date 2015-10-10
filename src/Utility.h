#pragma once
#include <string>
#include <map>
#include <vector>

template<typename T,typename T2> 
std::vector<T> GetMapKeys(const std::map<T,T2>& _map);

template<typename T,typename T2> 
std::vector<T> GetMapKeys( const std::map<T,T2>& _map )
{
	std::map<T,T2>::const_iterator it;
	std::vector<T> _return;
	for(it=_map.begin();it!=_map.end();++it)
	{
		_return.push_back(it->first);
	}
	return _return;
}
