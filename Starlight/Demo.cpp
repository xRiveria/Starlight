#include "Demo.h"
#include <memory>
#include <string>
#include "RTTI/Reflect.hpp"
#include <json/json.hpp>
#include "IO/FileSystem.h"
#include "IO/StringUtilities.h"
#include "Compression/Huffman.h"
#include "Serializations/Serializers/YAMLSerializer.h"
#include "Serializations/Serializer.h"

using nlohmann::json;

void Print(std::ostream& os, RTTI::Any object)
{
	const RTTI::TypeDescriptor* typeDescriptor = object.GetType();
	os << typeDescriptor->GetName() << "::" << "\n";

	for (RTTI::DataMember* dataMember : typeDescriptor->GetDataMembers())
	{
		const RTTI::TypeDescriptor* dataMemberType = dataMember->GetType();

		if (const RTTI::Function* memberFunction = typeDescriptor->GetMemberFunction("print"); memberFunction)
		{
			memberFunction->Invoke(RTTI::AnyRef(), dataMember->Get(object));
		}
		else
		{
			Print(os, dataMember->Get(object));
		}
	}
}

template <typename T>
void Print(const T& t)
{
	std::cout << RTTI::GetType<T>()->GetName() << ": " << t << std::endl;
}

class Pokemon
{
public:
	Pokemon() {}
	Pokemon(const std::string& pokemonName) : m_PokemonName(pokemonName) { }

	void Print(const std::string& name)
	{
		std::cout << name << "\n";
		std::cout << m_PokemonName << "\n";
	}

	float GetPokemonHealth() const { return m_PokemonHealth; }
	void SetPokemonHealth(float newHealth) { m_PokemonHealth = newHealth; }

	std::string GetName() const { return m_PokemonName; }

public:
	std::string m_PokemonName = "Pikachu";

private:
	float m_PokemonHealth = 0.0f;
};

void to_json(json& j, const Pokemon& a)
{
	j["PokemonName"] = a.m_PokemonName;
	j["PokemonHealth"] = a.GetPokemonHealth();
}

void from_json(const json& j, Pokemon& a)
{
	a.m_PokemonName = j.at("PokemonName");
	a.SetPokemonHealth(j.at("PokemonHealth"));
}

void SerializePokemon(json& json, const Pokemon& pokemon)
{
	json = pokemon;
}

void DeserializePokemon(const json& json, Pokemon& pokemon)
{
	pokemon = json;
}

void SerializationTest()
{
	// reflect types
	RTTI::Reflect<int>("int")
		.AddMemberFunction(&Print<int>, "print");

	RTTI::Reflect<float>("float")
		.AddMemberFunction(&Print<float>, "print");

	RTTI::Reflect<std::string>("string")
		.AddConstructor<const char*>()
		.AddMemberFunction(&Print<std::string>, "print");

	RTTI::Reflect<double>("double")
		.AddConversion<int>();

	RTTI::Reflect<const char*>("cstring")
		.AddConversion<std::string>();

	// Register class with our reflection.
	RTTI::Reflect<Pokemon>("Pokemon")
		.AddConstructor<>()
		.AddConstructor<const std::string&>()
		.AddDataMember(&Pokemon::m_PokemonName, "PokemonName")
		.AddDataMember<&Pokemon::SetPokemonHealth, &Pokemon::GetPokemonHealth>("Health")
		.AddMemberFunction(&Pokemon::Print, "Print")
		.AddMemberFunction(&SerializePokemon, "Serialize")
		.AddMemberFunction(&DeserializePokemon, "Deserialize");

	auto pokemon = RTTI::GetType("Pokemon")->GetConstructor<const std::string&>()->NewInstance("Raichu");
	RTTI::GetType("Pokemon")->GetMemberFunction("Print")->Invoke(pokemon, "This Pokemon Name Is");
	RTTI::GetType("Pokemon")->GetDataMember("Health")->Set(pokemon, 500.0f);

	Print(std::cout, pokemon);

	// Serialize Existing Data
	std::cout << "Serialization\n";
	json jj;
	RTTI::GetType("Pokemon")->GetMemberFunction("Serialize")->Invoke(RTTI::AnyRef(), RTTI::AnyRef(jj), pokemon);
	std::cout << "Json Data: " << jj << "\n";

	// Deserialize Data
	auto pokemon2 = RTTI::GetType("Pokemon")->GetConstructor<>()->NewInstance();
	RTTI::GetType("Pokemon")->GetMemberFunction("Deserialize")->Invoke(RTTI::AnyRef(), jj, RTTI::AnyRef(pokemon2));
	RTTI::GetType("Pokemon")->GetMemberFunction("Print")->Invoke(pokemon2, RTTI::Any("Deserializing Time!"));

	static_cast<Pokemon*>(pokemon2.Get())->Print("Type Cast");

	// C++ Types
	auto stringTest = RTTI::GetType("string")->GetConstructor<const char*>()->NewInstance("Hello");
	std::cout << *stringTest.TryCast<std::string>();
}

namespace Application
{
	std::shared_ptr<Starlight::Logger> Log::s_ApplicationLogger = nullptr;

	void Log::InitializeLogger()
	{
		s_ApplicationLogger = std::make_shared<Starlight::Logger>("Aurora Engine"); //Creates a global logger object.
	}
}

int main(int argc, int argv[])
{
	Serializer::Serialize();

	// Node that the string's storage is internally its string characters * 8 bits. 47 x 8 = 376 bits, but the encoded string only takes 194 bits (48% of compression).
	std::string textToCompress = "Huffman coding is a data compression algorithm.";

	TestCompression(textToCompress);

	IO::FileSystem::SetProjectDirectory("");
	if (IO::FileSystem::Exists("Demo.h"))
	{
		std::cout << "Exists!\n";
	}

	auto filePaths = IO::FileSystem::GetDirectoriesInDirectory("");
	for (auto& path : filePaths)
	{
		std::cout << StringUtilities::ConvertToLowercase(IO::FileSystem::GetFileNameWithoutExtensionFromFilePath(path)) << "\n";
	}

	IO::FileSystem::Delete("Peep.txt");
	// IO::FileSystem::CreateDirectory_("Derp");
	// IO::FileSystem::CreateTextFile("Hello.txt", "dwdqdwrfrfw");


	//Example way to setup the system for use with your engine. 
	Application::Log::InitializeLogger();

	//File Logging Support
	{
		//Enables support to store your logs into a text file. This is a master switch that does nothing on its own. We must also enable its associative features. 
		Application::Log::GetInstance()->EnableFileLogging(true); 
		//Creates a new log file everytime the application launches.
		Application::Log::GetInstance()->EnableStartupLogging(true);
		//Starts a daily timer that creates a log file at 5:30am everyday.
		Application::Log::GetInstance()->EnableDailyLoggingTimer(true, 5, 30);
		//Changes the log directory for your text logs.
		// Starlight::FileSystem::ChangeLogDirectory("LogsFolder/");
	}

	//Backtracing Support
	{
		//Enables support to store your logs into a buffer to be retrieved at will. Second argument states the amount of messages to be stored up to at any given point.
		Application::Log::GetInstance()->EnableBacktracing(true, 5);
	}

	//Different ways to log your system.
	{
		ENGINE_LOG_INITIALIZATION("Initialized %s", "Starlight Logger!");
		ENGINE_LOG_INFO("%d enemies destroyed in this level.", 51);
		ENGINE_LOG_WARN("%d GPU Cycles Recorded! Bad!", 5);
		ENGINE_LOG_ERROR("Your backend named %s has crashed!", "Aurora");
		//ENGINE_LOG_CRITICAL("Core system error!"); //Critical errors will terminate your program.
	}

	//Outputs the messages you've stored in a backbuffer if enabled.
	std::cout << "Dumping backtrace buffer..." << "\n";
	Application::Log::GetInstance()->DumpBacktracingBuffer();

	std::cerr << "Error!\n"; // Unbuffered standard stream. Displays the error message immediately.
}
