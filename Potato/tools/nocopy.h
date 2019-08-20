#ifndef NOCOPY_H
#define NOCOPY_H

#pragma once
namespace Potato
{
	class nocopy
	{
	protected:
		nocopy() {};
		virtual ~nocopy() {};
	private:
		nocopy(const nocopy&);
		const nocopy& operator=(const nocopy&);
	};
}

#endif // !NOCOPY_H
