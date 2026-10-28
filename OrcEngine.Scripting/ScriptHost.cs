using System.Runtime.InteropServices;

namespace OrcEngine.Scripting;

public static class ScriptHost
{
    [UnmanagedCallersOnly]
    public static unsafe IntPtr CreateInstance(byte* typeNameUtf8)
    {
        string typeName = Marshal.PtrToStringUTF8((IntPtr)typeNameUtf8)!;
        Type? type = Type.GetType($"OrcEngine.Scripting.{typeName}, OrcEngine.Scripting");
		
        if (type == null || !typeof(Script).IsAssignableFrom(type))
            return IntPtr.Zero;

        var instance = (Script)Activator.CreateInstance(type)!;
        instance.OnCreate();

        // Allocate a GCHandle to track the object without pinning memory
        GCHandle handle = GCHandle.Alloc(instance, GCHandleType.Normal);
        return GCHandle.ToIntPtr(handle);
    }

    [UnmanagedCallersOnly]
    public static void OnUpdate(IntPtr handlePtr, float deltaTime)
    {
        if (handlePtr == IntPtr.Zero) return;

        GCHandle handle = GCHandle.FromIntPtr(handlePtr);
        if (handle.Target is Script script)
        {
            script.OnUpdate(deltaTime);
        }
    }

    [UnmanagedCallersOnly]
    public static void DestroyInstance(IntPtr handlePtr)
    {
        if (handlePtr == IntPtr.Zero) return;

        GCHandle handle = GCHandle.FromIntPtr(handlePtr);
        if (handle.Target is Script script)
        {
            script.OnDestroy();
        }

        // Free the handle so the GC can safely collect the instance
        handle.Free();
    }
}