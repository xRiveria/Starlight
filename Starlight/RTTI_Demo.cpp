#include "RTTI/Reflect.hpp"
#include "RTTI/json.hpp"
#include <string>
#include <iostream>

struct Vector3D
{
	float x;
	float y;
	float z;
};

class Player
{
public:
	Player() : id(10) {}
	Player(int id) : id(id) {}
	Player(int id, const std::string &name) : id(id), name(name) {}
	Player(Vector3D const &position) : position(position) {}

	Player GetThis() const { return *this; }

	void SetId(int id) { Player::id = id; }
	int GetId() const { return id; }

	void SetName(const std::string &name) { this->name = name; }
	std::string GetName() const { return name; }

	void SetPosition(Vector3D position) { this->position = position; }
	Vector3D GetPosition() const { return position; }

	void Print(const std::string &s) const
	{
		std::cout << s << '\n';
		std::cout << "player id: " << id << ", name: " << name << ", health: " << health << '\n';
		std::cout << "position: " << position.x << ", y: " << position.y << ", z: " << position.z << std::endl;
		std::cout << std::endl;
	}

	std::string SayHello() const { return "hello from " + name + "!"; }

	float health = 100.0f;

	int *iptr;
private:
	int id = 11;
	std::string name = "Mario";
	Vector3D position;
};

Player PlayerCtorFun(const std::string &name, int id)
{
	Player player(42, "Geralt");

	return player;
}

using nlohmann::json;

void to_json(json &j, const Vector3D &a)
{
	j["x"] = a.x;
	j["y"] = a.y;
	j["z"] = a.z;
}

void from_json(const json &j, Vector3D &a)
{
	j.at("x").get_to(a.x);
	j.at("y").get_to(a.y);
	j.at("z").get_to(a.z);
}

void to_json(json &j, const Player &a)
{
	j["id"] = a.GetId();
	j["name"] = a.GetName();
	j["position"] = a.GetPosition();
	j["health"] = a.health;
}

void from_json(const json &j, Player &a)
{
	a.SetId(j.at("id").get<int>());
	a.SetName(j.at("name").get<std::string>());
	a.SetPosition(j.at("position").get<Vector3D>());
	a.health = j.at("health");
}

void Serialize(json &j, const Player &a)
{
	j = a;
}

void Deserialize(const json &j, Player &a)
{
	a = j;
}

void Print(std::ostream &os, Reflect::Any object)
{
	Reflect::TypeDescriptor const *td = object.GetType();
	os << td->GetName() << "::" << std::endl;

	for (auto *dataMember : td->GetDataMembers())
	{
		auto *ftd = dataMember->GetType();

		//if (ftd == Reflect::Resolve<int>())
		//	os << "int: " << *dataMember->Get(object).TryCast<int>() << std::endl;
		//else if (ftd == Reflect::Resolve<float>())
		//	os << "float: " << *dataMember->Get(object).TryCast<float>() << std::endl;
		//else if (ftd == Reflect::Resolve<std::string>())
		//	os << "string: " << *dataMember->Get(object).TryCast<std::string>() << std::endl;

		if (auto mf = ftd->GetMemberFunction("print"); mf)
			mf->Invoke(Reflect::AnyRef(), dataMember->Get(object));
		else
			Print(os, dataMember->Get(object));
	}
}

template <typename T>
void Print(const T &t)
{
	std::cout << Reflect::Resolve<T>()->GetName() << ": " << t << std::endl;
}

// useless ?
//template <typename T>
//void PrintAny(Reflect::Any any)
//{
//	std::cout << any.GetType()->GetName() << ": " << any.TryCast<T>() << std::endl;
//}


class Pokemon
{
public:
	Pokemon(const std::string& pokemonName) : m_PokemonName(pokemonName) { }

	void Print(const std::string& name)
	{
		std::cout << name << "\n";
		std::cout << m_PokemonName << "\n";
	}

public:
	std::string m_PokemonName;
};

void SerializePokemon(json& json, Pokemon* pokemon)
{

}

int main(int argc, char **argv)
{
	// reflect types
	Reflect::Reflect<int>("int")
		.AddMemberFunction(&Print<int>, "print");

	Reflect::Reflect<float>("float")
		.AddMemberFunction(&Print<float>, "print");

	Reflect::Reflect<std::string>("string")
		.AddConstructor<const char*>()
		.AddMemberFunction(&Print<std::string>, "print");

	Reflect::Reflect<double>("double")
		.AddConversion<int>();

	Reflect::Reflect<const char*>("cstring")
		.AddConversion<std::string>();






	Reflect::Reflect<Pokemon>("Pokemon").AddConstructor<const std::string&>().AddDataMember(&Pokemon::m_PokemonName, "PokemonName").AddMemberFunction(&Pokemon::Print, "Print");
	auto pokemon = Reflect::Resolve("Pokemon")->GetConstructor<const std::string&>()->NewInstance("Raichu");
	Reflect::Resolve("Pokemon")->GetMemberFunction("Print")->Invoke(pokemon, "This Pokemon Name Is");

	// Serialize Existing Data










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

	return 0;
}