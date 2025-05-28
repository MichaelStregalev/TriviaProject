using BackendTrivia;
using System.Text;
using System.Text.Json;

class Program
{
    static void Main()
    {
        Login log = new Login();
        log.LoginAcc("Steven", "dogno");
    }
}

//class Program
//{
//    static void Main()
//    {
//        var data = new
//        {

//        };

//        // Serialize to JSON
//        string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

//        string message = "2" + (json.Length).ToString() + json;

//        Console.WriteLine(message);
//    }
//}