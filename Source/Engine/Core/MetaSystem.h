
#ifndef metasystem_h
#define metasystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Core/Variant.h>
#include <Core/VariantRef.h>
#include <Core/GigaObject.h>

typedef GigaObject* (*MetaConstructor)(void);

class GIGA_API MetaSystem : public System {
public:
	MetaSystem() = default;
	~MetaSystem() = default;
    
    GIGA_CLASS_NAME("MetaSystem");
    
    /**
     * Initialize meta functions
     */
    void Initialize();
    
    /**
     * Register all meta functions (auto-generated)
     */
    void RegisterMetaFunctions();

	/**
	 * Register a class-specific callback
	 */
	void RegisterFunction(std::string className, std::string functionName, CallableFunction c, bool isStatic);

	/**
	 * Find a class-specific callback function
	 */
	CallableFunction FindFunction(std::string className, std::string functionName);
    
    /**
     * Register a variable
     */
    template <class T>
    void RegisterVariable(std::string className, std::string name, VariantRef<T> ref, bool settable, bool gettable) {
        std::map<std::string, RegisteredClass*>::iterator it = m_classes.find(className);
        GIGA_ASSERT(it != m_classes.end(), "Class type not registered.");
        
        std::map<std::string, RegisteredVariable*>::iterator fi = it->second->variables.find(name);
        GIGA_ASSERT(fi == it->second->variables.end(), "Variable already registered.");
        
        RegisteredVariable* var = new RegisteredVariable();
        var->var = ref;
        var->name = name;
        var->gettable = gettable;
        var->settable = settable;
        
        it->second->variables[name] = var;
    }

	/**
	 * Create a new object of type className
	 */
	GigaObject* CreateObject(std::string className);

	/**
	 * Register a new class type
	 */
	template<class T> void RegisterClassName(std::string className) {
		std::map<std::string, RegisteredClass*>::iterator it = m_classes.find(className);
		GIGA_ASSERT(it == m_classes.end(), "Class type already registered.");

		RegisteredClass* cl = new RegisteredClass();
		cl->name = className;
		cl->ctor = &CreateGigaObject<T>;

		m_classes[className] = cl;
	}

	/**
	 * Register a singleton instance
	 */
	void RegisterSingleton(std::string className, GigaObject* instance);

	/**
	 * Get singleton instance
	 */
	GigaObject* GetSingleton(std::string className);
    
    /**
     * Get registered classes
     */
    struct RegisteredClass;
    std::vector<RegisteredClass*> GetRegisteredClasses();

	struct RegisteredFunction {
		std::string name;
		CallableFunction func;
		bool isStatic;
	};
    
    struct RegisteredVariable {
        std::string name;
        VariantRef var;
        bool gettable;
        bool settable;
    };

	struct RegisteredClass {
		std::string name;

		// Constructor
		MetaConstructor ctor;

		// Callable meta functions
		std::map<std::string, RegisteredFunction*> functions;
        
        // Variables
        std::map<std::string, RegisteredVariable*> variables;
	};

protected:
	// Create a new C++ object based on a registered script interface type
	template<typename T> static GigaObject* CreateGigaObject() { return new T; }

protected:
	// Registered classes
	std::map<std::string, RegisteredClass*> m_classes;

	// Registered singletons
	std::map<std::string, GigaObject*> m_singletons;
};

#endif
