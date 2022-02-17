// pre-declaration of a class, for its use before definition 
class Executor;  

class Initiator  
{
public:

    // the type of the pointer to the function is declared , which contains the context
    using ptr_callback_static = void(*) (int, Executor*);  

    // function for setting pointers
    void setup(ptr_callback_static pPtrCallback, Executor* pContextData) // mark 3
    {
    	// saving the callback
        ptrCallback = pPtrCallback;  // mark 4 
	
	// pointer to context !
        contextData = pContextData;  
    }

    // 
    void run()  // mark 6 
    {
        int eventID = 0; 
        
        // Some actions
        // Some actions
        // Some actions 
        
        // run Callvack with context 
        // it turns out that the pointer to the 
        // callback <pPtrCallback> and to the context <Executor> 
        // passed at setup time.
        
        ptrCallback(eventID, contextData); 
        
        // makr 8  We're going back.
        
    }

private:
    ptr_callback_static ptrCallback = nullptr;  
    Executor* contextData = nullptr;  
};


class Executor  // (1)
{
public:
    // setting up a callback
    Executor(Initiator* initiator)  
    {
    	// settins initiator / 
        initiator->setup(callbackHandler, this); // mark 2
    }

    // handler call back 
    static void callbackHandler(int eventID, Executor* executor)  
    {
        //It will be called by initiator
        executor->onCallbackHandler(eventID); 
        // the input parameter is the call information < eventID >
        // pointer to the context <Executor>
        
        // !!! внутри метода можно обращатся к содержимому класса, по указателю который является квалификатором. 
        // inside the method, you can access the contents of the class by a pointer that is a qualifier
        
    }

private:
    void onCallbackHandler(int eventID)  
    {
    	
    	// makr  7 
        //Do what is necessary
    }
};

int main() 
{
    Initiator initiator; // mark 0 
    Executor executor(&initiator);  // makr 1
    initiator.run(); // mark 5
    // mark 9
    //Wait finish
}
