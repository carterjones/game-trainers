using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Nouzuru;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Diagnostics;

class Program
{
    static void Main(string[] args)
    {
        Patcher p = new Patcher();
        if (!p.Open("PwnAdventure3-Win32-Shipping"))
        {
            Console.WriteLine("Failed to open PwnAdventure3");
            return;
        }

        ProcessModule gameLogic = p.GetModuleByName("GameLogic.dll");
        int sprintMultiplier   = 0x00078B34;
        int getJumpSpeedOffset = 0x0004EFA0;

        // Super Speed
        byte[] superSpeedBytes = new byte[] { 0x00, 0x00, 0x20, 0x41 };
        IntPtr targetAddress = IntPtr.Add(gameLogic.BaseAddress, sprintMultiplier);
        if (!p.Write(targetAddress, superSpeedBytes))
        {
            int lastError = Marshal.GetLastWin32Error();
            string errorMessage1 = new Win32Exception(lastError).Message;
            Console.WriteLine(errorMessage1);
        }

        // Moon Jump
        //byte[] moonjumpBytes = new byte[] { 0x00, 0x00, 0x20, 0x41 };
        //p.Write(IntPtr.Add(p.BaseAddress, getJumpSpeedOffset), 0x90);

        return;
    }
}