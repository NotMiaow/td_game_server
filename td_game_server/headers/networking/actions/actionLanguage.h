#ifndef ACTION_LANGUAGE_H__
#define ACTION_LANGUAGE_H__

enum ActionType
{
	AError,
	ACreateAction,
	AConnect,
	ADisconnect,
	AGameAction
};

enum NetworkingErrorType
{
	NEWrongActionType,
	NEForbidden,
	NEWrongParemeterAmount,
	NEWrongParameterType,
	NEWrongSessionToken
};

#endif