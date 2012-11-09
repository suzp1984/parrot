import java.net.*;
class FortuneClient
{
	public static void main(String args[])
	{
		FortuneClient client = new FortuneClient();
		System.out.println(client.getMessage());
	}
	public String getMessage()
	{
		String fortune;
		try {
			DatagramSocket socket;
			DatagramPacket packet;
			byte[] data = new byte[108];

			for(int i=0;i<(data.length);i++){
				Integer integer=new Integer(i);
				data[i]=integer.byteValue();
			}

			socket = new DatagramSocket(8888);
			//socket.bind(new InetSocketAddress(8888));
			//packet = new DatagramPacket(data, data.length,InetAddress.getByName("127.0.0.1"), 1114);
			packet = new DatagramPacket(data, data.length,InetAddress.getLocalHost(), 1114);
			socket.send(packet);

			packet = new DatagramPacket(data, data.length);
			socket.receive(packet);
			System.out.println("get the Server port is: " + packet.getPort());
			System.out.println("get the data length is: " + packet.getData().length);
			fortune = new String(packet.getData());
			socket.close();
		}
		catch (UnknownHostException e) {
			System.err.println("Exception: host could not be found");
			return null;
		}
		catch (Exception e) {
			System.err.println("Exception: " + e);
			e.printStackTrace();
			return null;
		}
		return fortune;
	}
} 
