using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BackendTrivia
{
    public class Room
    {
        private Communicator mCom;
        public Room(Communicator c)
        {
            mCom = c;
        }
        public Communicator GetCommunicator()
        {
            return this.mCom;
        }

    }
}
