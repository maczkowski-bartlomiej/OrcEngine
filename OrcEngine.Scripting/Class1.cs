using System;
using System.Runtime.InteropServices;

namespace OrcEngine.Scripting;

public static class Class1
{
	[UnmanagedCallersOnly]
	public static void Hello()
	{
		Console.WriteLine("[Managed .NET 10] Hello from C# Scripting inside OrcEngine!");
	}
}

/*

public class RenderTest : public GameLayer
{
	void onUpdate(float dt)
	{
	}
	
	void onRender()
	{
		OrcEngine.draw(m_sprite`
		
		+
		
		
		
		
		
		
		
	}
}

*/