using System;
using System.Collections.Generic;
using System.Text;

namespace AGS.Editor
{
	public abstract class BaseTextProcess
	{
		protected Dictionary<string, FunctionCallType> GetFunctionCallsToProcessForSpeech(bool includeNarrator)
		{
			Dictionary<string, FunctionCallType> speechableFunctionCalls = new Dictionary<string, FunctionCallType>();
			speechableFunctionCalls.Add("DisplaySpeech", FunctionCallType.GlobalSpeech);
			speechableFunctionCalls.Add("DisplayThought", FunctionCallType.GlobalSpeech);
            //speechableFunctionCalls.Add("SayText", FunctionCallType.GlobalSpeech);
            speechableFunctionCalls.Add(".AddTapeMessage", FunctionCallType.ObjectBasedSpeech);
            speechableFunctionCalls.Add(".AddToVOScript", FunctionCallType.ObjectBasedSpeech);
            speechableFunctionCalls.Add(".GSay", FunctionCallType.ObjectBasedSpeech);
            speechableFunctionCalls.Add(".Say", FunctionCallType.ObjectBasedSpeech);
            speechableFunctionCalls.Add(".SayBackground", FunctionCallType.ObjectBasedSpeech);
			speechableFunctionCalls.Add(".Think", FunctionCallType.ObjectBasedSpeech);
			if (includeNarrator)
			{
				speechableFunctionCalls.Add("Display ", FunctionCallType.GlobalNarrator);
				speechableFunctionCalls.Add("Display(", FunctionCallType.GlobalNarrator);
				speechableFunctionCalls.Add("DisplayAt", FunctionCallType.GlobalNarrator);
			}
			return speechableFunctionCalls;
		}
	}
}
