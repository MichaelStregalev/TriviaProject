using BackendTrivia;
using System.Text.Json;

//class Program
//{
//    static void Main()
//    {
//        Communicator comm = new();

//    }
//}

var data = new
{
    userName = "Steven",
    password = "DamSon"
};

// Serialize to JSON
string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

string message = "2" + (json.Length).ToString() + json;

Console.WriteLine(message);