#ifndef GALAXY_ERRORS_H
#define GALAXY_ERRORS_H

/**
 * @file
 * Contains classes representing exceptions.
 */

#include "GalaxyExport.h"

namespace galaxy
{
	namespace api
	{
		/**
		 * @addtogroup api
		 * @{
		 */

		/**
		 * Base interface for exceptions.
		 */
		class IError
		{
		public:

			virtual ~IError()
			{
			}

			/**
			 * Returns the name of the error.
			 *
			 * @return The name of the error.
			 */
			virtual const char* GetName() const = 0;

			/**
			 * Returns the error message.
			 *
			 * @return The error message.
			 */
			virtual const char* GetMsg() const = 0;

			/**
			 * Type of error.
			 */
			enum Type
			{
				UNAUTHORIZED_ACCESS,
				INVALID_ARGUMENT,
				INVALID_STATE,
				RUNTIME_ERROR
			};

			/**
			 * Returns the type of the error.
			 *
			 * @return The type of the error.
			 */
			virtual Type GetType() const = 0;
		};

		/**
		 * The exception thrown when calling Galaxy interfaces while
		 * the user is not signed in and thus not authorized for any interaction.
		 */
		class IUnauthorizedAccessError : public IError
		{
		};

		/**
		 * The exception thrown to report that a method was called with an invalid argument.
		 */
		class IInvalidArgumentError : public IError
		{
		};

		/**
		 * The exception thrown to report that a method was called while the callee is in
		 * an invalid state, i.e. should not have been called the way it was at that time.
		 */
		class IInvalidStateError : public IError
		{
		};

		/**
		 * The exception thrown to report errors that can only be detected during runtime.
		 */
		class IRuntimeError : public IError
		{
		};

		/**
		 * Retrieves error connected with the last API call on the local thread.
		 *
		 * @return Either the last API call error or NULL if there was no error.
		 */
		GALAXY_DLL_EXPORT const IError* GALAXY_CALLTYPE GetError();

		/** @} */
	}
}

#endif
