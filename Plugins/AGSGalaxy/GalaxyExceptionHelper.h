#ifndef GALAXY_EXCEPTION_HELPER_H
#define GALAXY_EXCEPTION_HELPER_H

#include "Errors.h"
#include <string>

namespace galaxy
{
	namespace api
	{
		namespace details
		{

#define GALAXY_EXCEPTION_HELPER_ERROR_CLASS(Exception, ExceptionInterface, ErrorType) \
class Exception : public ExceptionInterface \
{\
public: \
	explicit Exception(const IError* exception) : message(exception->GetMsg()) {}\
	const char* GetName() const override { return #ExceptionInterface; } \
	const char* GetMsg() const override { return message.c_str(); } \
	api::IError::Type GetType() const override { return ErrorType; } \
\
private: \
	const std::string message; \
}

			GALAXY_EXCEPTION_HELPER_ERROR_CLASS(UnauthorizedAccessError, IUnauthorizedAccessError, IError::UNAUTHORIZED_ACCESS);
			GALAXY_EXCEPTION_HELPER_ERROR_CLASS(InvalidArgumentError, IInvalidArgumentError, IError::INVALID_ARGUMENT);
			GALAXY_EXCEPTION_HELPER_ERROR_CLASS(InvalidStateError, IInvalidStateError, IError::INVALID_STATE);
			GALAXY_EXCEPTION_HELPER_ERROR_CLASS(RuntimeError, IRuntimeError, IError::RUNTIME_ERROR);

		}

		inline void ThrowIfGalaxyError()
		{
			const IError* error = GetError();
			if (error)
			{
				switch (error->GetType())
				{
					case IError::UNAUTHORIZED_ACCESS: throw details::UnauthorizedAccessError(error);
					case IError::INVALID_ARGUMENT: throw details::InvalidArgumentError(error);
					case IError::INVALID_STATE: throw details::InvalidStateError(error);
					default: throw details::RuntimeError(error);
				}
			}
		}

	}
}

#endif
