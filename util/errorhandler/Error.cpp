#include "Error.h"
#include "Logger.h"

namespace sys {


	std::vector<Error> ErrorHandler::m_errors;
	std::vector<ErrorHandler::Action> ErrorHandler::m_actions;

	Error::Error(int errorCode, const std::string& desc) :
		errorCode{ errorCode }, what{ desc }
	{
	}

	std::string Error::What()
	{
		return what;
	}


	int Error::Errorcode()
	{
		return errorCode;
	}

	void ErrorHandler::AddError(int errorCode, const std::string& desc)
	{
		m_errors.push_back(Error{ errorCode, desc });
	}

	void ErrorHandler::Handle()
	{
		for (Error& error : m_errors)
		{
			switch (error.Errorcode())
			{
			case Error::WINDOW_CONSTRUCTION_FAILED: WindowConstructionFailed();
			}
			VSTM_DEBUG_LOGERROR("[VSTM Error]\nError code : {}\nError description: {}\n", error.Errorcode(), error.What());
		}
		m_errors.clear();
	}

	size_t ErrorHandler::NumActions()
	{
		return m_actions.size();
	}

	ErrorHandler::Action ErrorHandler::GetAction(size_t i)
	{
		return m_actions[i];
	}

	void ErrorHandler::WindowConstructionFailed()
	{
		ErrorMessasgeBox("Couldn't create window", "Window construction failed.\nApplication closed.");
		m_actions.push_back(TerminateApplication);
	}

}
