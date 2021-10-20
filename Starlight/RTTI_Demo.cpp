#include "RTTI/Reflect.hpp"
#include "RTTI/json.hpp"
#include <string>
#include <iostream>

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
void Print(const T &t)
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

int main(int argc, char **argv)
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



	/*
	Reflect::Reflect<Vector3D>("Vector3D")
		.AddDataMember(&Vector3D::x, "x")
		.AddDataMember(&Vector3D::y, "y")
		.AddDataMember(&Vector3D::z, "z");

	Reflect::Reflect<Player>("Player")
		.AddConstructor<>()
		.AddConstructor<int>()
		.AddConstructor<int, const std::string&>()
		.AddConstructor<Vector3D const &>()
		.AddConstructor(&PlayerCtorFun)
		.AddDataMember<&Player::SetId, &Player::GetId>("id")
		.AddDataMember<&Player::SetName, &Player::GetName>("name")
		.AddDataMember <&Player::SetPosition, &Player::GetPosition>("position")
		.AddDataMember(&Player::health, "health")
		.AddDataMember(&Player::iptr, "iptr")
		.AddMemberFunction(&Player::Print, "Print")
		.AddMemberFunction(&Player::GetThis, "get player copy")
		.AddMemberFunction(&Serialize, "Serialize")
		.AddMemberFunction(&Deserialize, "Deserialize");

	Reflect::typeFactory<Player>.AddMemberFunction(&Player::SayHello, "SayHello");

	//Reflect::Any a = Reflect::Resolve("Player")->GetConstructor<int, const std::string &>()->NewInstance(1, "Mario");
	Reflect::Any a = Reflect::Resolve("Player")->GetConstructor<const std::string &, int>()->NewInstance("Mario", 1);
	Reflect::Resolve("Player")->GetDataMember("position")->Set(a, Vector3D{ 13.4f, 15.0f, 11.1f });
	//Reflect::Resolve("Player")->GetDataMember("id")->Set(a, 1.7);
	//Reflect::Resolve("Player")->GetDataMember("name")->Set(a, "Luigi");
	Reflect::Resolve("Player")->GetDataMember("health")->Set(a, 20.0f);
	Reflect::Resolve("Player")->GetMemberFunction("Print")->Invoke(a, "player A stats:");  // <--- Invoke with custom arguments

	int i = 10;
	Reflect::Resolve("Player")->GetDataMember("iptr")->Set(a, &i);
	std::cout << **Reflect::Resolve("Player")->GetDataMember("iptr")->Get(a).TryCast<int*>() << std::endl;


	auto b = Reflect::Resolve("Player")->GetConstructor<const Vector3D&>()->NewInstance(Vector3D{ 0.2f, 1.3f, 2.2f });
	Reflect::Resolve("Player")->GetMemberFunction("Print")->Invoke(b, Reflect::Any("player B stats:"));  // <--- Invoke with Any objects

	//std::cout << "player id: " << *Reflect::Resolve(Player())->GetDataMember("id")->Get(a).TryCast<int>() << std::endl;
	//std::cout << *Reflect::Resolve("Player")->GetMemberFunction("SayHello")->Invoke(a).TryCast<std::string>() << std::endl;
	//std::cout << *Reflect::Resolve("Player")->GetMemberFunction("SayHello")->Invoke(b).TryCast<std::string>() << std::endl;

	std::cout << "************** print *****************" << std::endl;
	Print(std::cout, b.GetType()->GetMemberFunction("get player copy")->Invoke(b));

	// print Player
	std::cout << "************** print *****************" << std::endl;
	Print(std::cout, a);

	// serialize/deserialize
	std::cout << "************** serialize/deserialize *****************" << std::endl;

	json j;
	Reflect::Resolve("Player")->GetMemberFunction("Serialize")->Invoke(Reflect::AnyRef(), Reflect::AnyRef(j), a);      // <--- Invoke with any argument

	std::cout << "json: " << j << std::endl;

	b = Reflect::Resolve("Player")->GetConstructor<>()->NewInstance();
	Reflect::Resolve("Player")->GetMemberFunction("Deserialize")->Invoke(Reflect::AnyRef(), j, Reflect::AnyRef(b));

	Reflect::Resolve("Player")->GetMemberFunction("Print")->Invoke(b, Reflect::Any("d'oh!"));


	auto s = Reflect::Resolve("string")->GetConstructor<const char*>()->NewInstance("hello");
	std::cout << *s.TryCast<std::string>() << std::endl;
	*/
	return 0;
}