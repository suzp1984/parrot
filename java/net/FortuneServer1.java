import java.io.*;
import java.net.*;
class FortuneServer1 extends Thread
{
	DatagramSocket ServerSocket;
	public FortuneServer1()
	{ 
		super("FortuneServer1");
		try
		{
			ServerSocket = new DatagramSocket(4000);
			System.out.println("FortuneServer up and running...");
		}
		catch (SocketException e)
		{
			System.err.println("Exception: couldn't create datagram socket");
			System.exit(1);
		}
	}
	public static void main(String[] args)
	{
		FortuneServer1 server = new FortuneServer1();
		server.start();
	}
	public void run()
	{
		if (ServerSocket == null)
			return;
		while (true)
		{
			try
			{
				InetAddress address;
				int port;
				DatagramPacket packet;
				byte[] data = new byte[1460];
				packet = new DatagramPacket(data, data.length);     
				ServerSocket.receive(packet);
				//
				//
				address = packet.getAddress();
				port = packet.getPort();
				System.out.println("get the Client port is: " + port);
				System.out.println("get the data length is: " + data.length);

				FileWriter fw = new FileWriter("Fortunes.txt");
				PrintWriter out = new PrintWriter(fw);    
				for(int i=0;i<data.length;i++){
					out.print(data[i]+"  ");
				}
				out.close();
				System.out.println("Data has been writen to destination!");

				packet = new DatagramPacket(data, data.length, address, port);
				ServerSocket.send(packet);
				System.out.println("Respond has been made!");
			}
			catch (Exception e)
			{
				System.err.println("Exception: " + e);
				e.printStackTrace();
			}
		}
	}
} 
