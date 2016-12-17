namespace Hl2Trainer
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Text;
    using Nouzuru;
    using Distorm3cs;

    public class Program
    {
        public static void Main(string[] args)
        {
            Hl2Trainer ht = new Hl2Trainer();
            ht.Open();

            ht.EnableUnlimitedSuitEnergy();
            ht.EnableUnlimitedAmmo();
            ht.EnableUnlimitedAuxiliaryPower();

            ht.EnableMassiveGrenadeCount = true;
            ht.EnableMassiveHealth = true;
            ht.EnableMassivePistolBackupAmmo = true;
            ht.EnableMassiveSuitEnergy = true;
            ht.StartFreezeThread();

            Console.WriteLine("Press any key to exit.");
            Console.ReadKey();
        }
    }
}
