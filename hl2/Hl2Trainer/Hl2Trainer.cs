namespace Hl2Trainer
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Threading;
    using Distorm3cs;
    using Nouzuru;

    /// <summary>
    /// The trainer used to modify the halo.exe process.
    /// </summary>
    public class Hl2Trainer : Patcher
    {
        #region Fields

        /// <summary>
        /// The address range of server.dll within the hl2.exe process. Do not call this directly.
        /// </summary>
        private Tuple<ulong, ulong> serverDllAddressRange;

        #endregion

        #region Constructors

        /// <summary>
        /// Initializes a new instance of the Hl2Trainer class.
        /// </summary>
        public Hl2Trainer()
        {
            this.EnableMassiveHealth = true;
            this.EnableMassiveSuitEnergy = true;
            unchecked
            {
                this.serverDllAddressRange = new Tuple<ulong, ulong>((ulong)-1, (ulong)-1);
            }
        }

        #endregion

        #region Properties

        /// <summary>
        /// Gets or sets a value indicating whether or not this trainer should grant the player massive health points.
        /// </summary>
        public bool EnableMassiveHealth { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether or not this trainer should grant the player massive amounts of
        /// suit energy.
        /// </summary>
        public bool EnableMassiveSuitEnergy { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether or not this trainer should grant the player massive amounts of
        /// pistol backup ammo.
        /// </summary>
        public bool EnableMassivePistolBackupAmmo { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether or not this trainer should grant the player massive amounts of
        /// grenades.
        /// </summary>
        public bool EnableMassiveGrenadeCount { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether or not this trainer should grant the player massive amounts of
        /// auxiliary power.
        /// </summary>
        public bool EnableMassiveAuxiliaryPower { get; set; }

        #endregion

        #region Methods

        /// <summary>
        /// Opens the halo.exe process.
        /// </summary>
        /// <returns>Returns true if the process was successfully opened.</returns>
        public bool Open()
        {
            if (!base.Open("hl2"))
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Enables unlimited health. This affects enemies as well, so it will be a very long game if this is enabled.
        /// </summary>
        /// <returns>Returns true if the enabling process was successful.</returns>
        public bool EnableUnlimitedHealth()
        {
            return this.EnableWithNOPInstruction(0x18bf89d5, "unlimited health");
        }

        /// <summary>
        /// Enables unlimited suit energy.
        /// </summary>
        /// <returns>Returns true if the enabling process was successful.</returns>
        public bool EnableUnlimitedSuitEnergy()
        {
            return this.EnableWithNOPInstruction(0x25ca002d, "unlimited suit energy");
        }

        /// <summary>
        /// Enables unlimited ammunition.
        /// </summary>
        /// <returns>Returns true if the enabling process was successful.</returns>
        /// <remarks>
        /// This function is slightly more complex, to provide an example of how to use various components of nouzuru
        /// in tandem. Using this type of method is more robust between updates, though not perfect. This is because
        /// it does not rely on certain memory addresses to remain constant, but instead relies on a particular
        /// function to remain constant.
        /// </remarks>
        public bool EnableUnlimitedAmmo()
        {
            // Enable unlimited ammo in a simple way.

            Console.Write("Enabling unlimited boat ammo...");
            if (!this.NOPInstruction(0x1894173f))
            {
                Console.WriteLine("Failure.");
            }
            else
            {
                Console.WriteLine("Success.");
            }

            Console.Write("Enabling unlimited secondary SMG ammo...");
            if (!this.NOPInstruction(0x18661afe))
            {
                Console.WriteLine("Failure.");
            }
            else
            {
                Console.WriteLine("Success.");
            }

            // Then, show an example of a more complex solution.

            Console.Write("Enabling general unlimited ammo...");

            Scanner s = new Scanner();
            s.Open("hl2");

            ProcessModuleCollection modules = s.GetLoadedModules();
            if (modules == null)
            {
                Console.Error.WriteLine("Failure. Please make sure that hl2.exe is running.");
                return false;
            }

            Tuple<ulong, ulong> serverDllAddress = this.GetServerDllAddressRange();

            s.IdentifyRegions(this.serverDllAddressRange.Item1, this.serverDllAddressRange.Item2);
            byte[] targetData = new byte[]
            {
                0x53, 0x8B, 0x5C, 0x24, 0x08, 0x57, 0x55, 0x8B, 0xF9, 0xBA, 0x04, 0x00, 0x00, 0x00, 0x8B, 0xC3, 0x56,
                0x8B, 0x31, 0x3B, 0x30, 0x75, 0x12, 0x83, 0xEA, 0x04, 0x83, 0xC0, 0x04, 0x83, 0xC1, 0x04, 0x83, 0xFA,
                0x04, 0x73, 0xEC, 0x85, 0xD2, 0x74, 0x5D, 0x0F, 0xB6, 0x31, 0x0F, 0xB6, 0x28, 0x2B, 0xF5, 0x75, 0x45,
                0x83, 0xEA, 0x01, 0x83, 0xC0, 0x01, 0x83, 0xC1, 0x01, 0x85, 0xD2, 0x74, 0x46, 0x0F, 0xB6, 0x31, 0x0F,
                0xB6, 0x28, 0x2B, 0xF5, 0x75, 0x2E, 0x83, 0xEA, 0x01, 0x83, 0xC0, 0x01, 0x83, 0xC1, 0x01, 0x85, 0xD2,
                0x74, 0x2F, 0x0F, 0xB6, 0x31, 0x0F, 0xB6, 0x28, 0x2B, 0xF5, 0x75, 0x17, 0x83, 0xEA, 0x01, 0x83, 0xC0,
                0x01, 0x83, 0xC1, 0x01, 0x85, 0xD2, 0x74, 0x18, 0x0F, 0xB6, 0x31, 0x0F, 0xB6, 0x00, 0x2B, 0xF0, 0x74,
                0x0E, 0x85, 0xF6, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x7F, 0x07, 0x83, 0xC8, 0xFF, 0xEB, 0x02, 0x33, 0xC0,
                0x85, 0xC0, 0x5E, 0x5D, 0x74, 0x33, 0x80, 0xBF, 0xE0, 0xFB, 0xFF, 0xFF, 0x00, 0x8D, 0x87, 0x8C, 0xFB,
                0xFF, 0xFF, 0x74, 0x0F, 0x80, 0x48, 0x58, 0x01, 0x8B, 0x03, 0x89, 0x07, 0x8B, 0xC7, 0x5F, 0x5B, 0xC2,
                0x04, 0x00, 0x8B, 0x48, 0x18, 0x85, 0xC9, 0x74, 0x0A, 0x8B, 0xD7, 0x2B, 0xD0, 0x52, 0xE8, 0x03, 0xCD,
                0xFE, 0xFF, 0x8B, 0x03, 0x89, 0x07, 0x8B, 0xC7, 0x5F, 0x5B, 0xC2, 0x04, 0x00, 0xCC
            };
            byte[] newData = new byte[]
            {
                0xEB, 0x07, 0x8B, 0xC7, 0x5F, 0x5B, 0xC2, 0x04, 0x00, 0xC7, 0x07, 0xE7, 0x03, 0x00, 0x00, 0xEB, 0xF1
            };

            if (!s.SearchLive(targetData))
            {
                Console.Error.WriteLine("Failure. Unable to find ammo increasing function. This could be because " +
                    "hl2.exe has been updated or because the target function has already been adjusted.");
                return false;
            }
            else
            {
                if (s.NumMatches == 1)
                {
                    IntPtr targetFunc = s.Matches[0];
                    Distorm.DInst[] insts = Distorm.Decompose(targetData);
                    foreach (Distorm.DInst inst in insts.Reverse())
                    {
                        if (inst.InstructionType == Distorm.InstructionType.MOV)
                        {
                            if (inst.ops[0].RegisterName == "edi" &&
                                inst.ops[0].type == Distorm.OperandType.SMEM &&
                                inst.ops[1].RegisterName == "eax")
                            {
                                ////if (!this.Write(IntPtr.Add(targetFunc, (int)inst.addr), newData, false, false))
                                if (!this.Write(IntPtr.Add(targetFunc, (int)inst.addr), newData, WriteOptions.SaveOldValue))
                                {
                                    Console.Error.WriteLine("Failure. Unable to replace target instructions.");
                                    return false;
                                }
                                else
                                {
                                    Console.WriteLine("Success.");
                                    return true;
                                }
                            }
                        }
                    }
                }
            }

            return false;
        }

        /// <summary>
        /// Enables unlimited auxiliary power.
        /// </summary>
        /// <returns>Returns true if the enabling process was successful.</returns>
        public bool EnableUnlimitedAuxiliaryPower()
        {
            return this.EnableWithNOPInstruction(0x25d6ea84, "unlimited auxiliary power");
        }

        /// <summary>
        /// Freeze the player's shields at 15 (normal is 1, overshield is 3).
        /// </summary>
        protected override void FreezeThread()
        {
            IntPtr baseAddress = this.GetPlayerBaseAddress();
            int healthOffset = Marshal.OffsetOf(typeof(GordonFreeman), "Health").ToInt32();
            int suitEnergyOffset = Marshal.OffsetOf(typeof(GordonFreeman), "SuitEnergy").ToInt32();
            int pistolBackupAmmoOffset = Marshal.OffsetOf(typeof(GordonFreeman), "PistolBackupAmmo").ToInt32();
            int grenadeCountOffset = Marshal.OffsetOf(typeof(GordonFreeman), "GrenadeCount").ToInt32();
            int auxiliaryPowerOffset = Marshal.OffsetOf(typeof(GordonFreeman), "AuxiliaryPower").ToInt32();

            byte[] newValue = BitConverter.GetBytes((short)999);

            while (true)
            {
                // Find the address again, in case a new level or level area was loaded.
                baseAddress = this.GetPlayerBaseAddress();

                if (!baseAddress.Equals(IntPtr.Zero))
                {
                    if (this.EnableMassiveHealth)
                    {
                        this.Write(IntPtr.Add(baseAddress, healthOffset), newValue);
                    }

                    if (this.EnableMassiveSuitEnergy)
                    {
                        this.Write(IntPtr.Add(baseAddress, suitEnergyOffset), newValue);
                    }

                    if (this.EnableMassivePistolBackupAmmo)
                    {
                        this.Write(IntPtr.Add(baseAddress, pistolBackupAmmoOffset), newValue);
                    }

                    if (this.EnableMassiveGrenadeCount)
                    {
                        this.Write(IntPtr.Add(baseAddress, grenadeCountOffset), newValue);
                    }

                    if (this.EnableMassiveAuxiliaryPower)
                    {
                        this.Write(IntPtr.Add(baseAddress, auxiliaryPowerOffset), newValue);
                    }
                }

                Thread.Sleep(this.FreezeFrequency);
            }
        }

        /// <summary>
        /// Gets the memory address range of server.dll within the hl2.exe process from the cache. If the cache has
        /// not been initialized, this function finds the proper value and caches it.
        /// </summary>
        /// <returns>
        /// Returns the address range with the smaller address as the first item and the larger address as the second
        /// item. Upon failure, a tuple with the value of (0,0) is returned and the cache remains unchanged.
        /// </returns>
        private Tuple<ulong, ulong> GetServerDllAddressRange()
        {
            // If the address has already been found, return it.
            unchecked
            {
                if (this.serverDllAddressRange.Item1 != (ulong)-1 ||
                    this.serverDllAddressRange.Item2 != (ulong)-1)
                {
                    return this.serverDllAddressRange;
                }
            }

            // Otherwise, discover the address and return it.
            ulong bottomAddress = 0, topAddress = 0;

            ProcessModuleCollection modules = this.GetLoadedModules();
            if (modules == null)
            {
                Console.Error.WriteLine("Please make sure that hl2.exe is running.");
                return new Tuple<ulong, ulong>(0, 0);
            }

            // Find the actual memory range of server.dll.
            bool serverDllFound = false;
            foreach (ProcessModule module in modules)
            {
                if (module.ModuleName.Equals("server.dll"))
                {
                    bottomAddress = (ulong)module.BaseAddress.ToInt64();
                    topAddress = bottomAddress + (ulong)module.ModuleMemorySize;
                    serverDllFound = true;
                    break;
                }
            }

            if (!serverDllFound)
            {
                Console.Error.WriteLine("Please make sure that hl2.exe has been fully loaded and that the player " +
                                        "is in a level within the game.");
                return new Tuple<ulong, ulong>(0, 0);
            }

            this.serverDllAddressRange = new Tuple<ulong, ulong>(bottomAddress, topAddress);
            return this.serverDllAddressRange;
        }

        /// <summary>
        /// Gets the base address of the current player's instance.
        /// </summary>
        /// <returns>
        /// Returns the base address of the player's class object, if a level is being played.
        /// Returns 0 if the player is in the home screen UI (ie: not in any level).
        /// </returns>
        private IntPtr GetPlayerBaseAddress()
        {
            if (!this.IsOpen)
            {
                return IntPtr.Zero;
            }

            ulong serverDllAddress = this.GetServerDllAddressRange().Item1;
            if (serverDllAddress == 0)
            {
                return IntPtr.Zero;
            }

            byte[] dynamicPivotAddressData = new byte[4];
            ulong baseAddress = serverDllAddress + 0x626104;
            this.Read(IntPtr.Add(IntPtr.Zero, (int)baseAddress), dynamicPivotAddressData);
            IntPtr dynamicPivotAddress = IntPtr.Add(IntPtr.Zero, BitConverter.ToInt32(dynamicPivotAddressData, 0));
            return dynamicPivotAddress;
        }

        /// <summary>
        /// Enables some advantage in halo, by modifying its memory.
        /// </summary>
        /// <param name="address">
        /// The address of the thing that restricts the player's awesomeness. The instruction at this address will be
        /// replaced with NOP instructions.
        /// </param>
        /// <param name="thingEnabled">The thing that makes the player more awesome.</param>
        /// <returns>Returns true if the enabling process was successful.</returns>
        private bool EnableWithNOPInstruction(ulong address, string thingEnabled)
        {
            Console.Write("Enabling " + thingEnabled + "...");
            if (this.NOPInstruction(address))
            {
                Console.WriteLine("Success.");
                return true;
            }
            else
            {
                Console.WriteLine("Failure.");
                return false;
            }
        }

        #endregion

        #region Structs

        /// <summary>
        /// Contains data about the current player, as it is represented in the game's memory.
        /// </summary>
        [StructLayout(LayoutKind.Explicit)]
        public struct GordonFreeman
        {
            /// <summary>
            /// The X axis of the character's in-game position.
            /// </summary>
            [FieldOffset(0xdc)]
            public float Health;

            /// <summary>
            /// The Y axis of the character's in-game position.
            /// </summary>
            [FieldOffset(0x244)]
            public float XAxisPosition;

            /// <summary>
            /// The Z axis of the character's in-game position.
            /// </summary>
            [FieldOffset(0x248)]
            public float YAxisPosition;

            /// <summary>
            /// The velocity the player is traveling along the X axis.
            /// </summary>
            [FieldOffset(0x24c)]
            public float ZAxisPosition;

            /// <summary>
            /// The number of frag grenades that the player currently has.
            /// </summary>
            [FieldOffset(0xcf4)]
            public byte SuitEnergy;

            /// <summary>
            /// The amount of shields the player has.
            /// </summary>
            [FieldOffset(0x6a4)]
            public float PistolBackupAmmo;

            /// <summary>
            /// A byte indicating the state of invisibility for the player.
            /// 0x51 indicates that the player is currently invisible.
            /// Any other value indicates that the player is currently visibile.
            /// </summary>
            [FieldOffset(0x6c8)]
            public byte GrenadeCount;

            /// <summary>
            /// The number of plasma grenades that the player currently has.
            /// </summary>
            [FieldOffset(0x1090)]
            public byte AuxiliaryPower;
        }

        #endregion
    }
}
