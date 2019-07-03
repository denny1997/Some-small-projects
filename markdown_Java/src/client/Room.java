package client;

import java.io.PrintWriter;
import java.util.ArrayList;

public class Room {
	private int mID;
	
	public Room(int id) {
		mID = id;
	}
	
	private Client mHost;	//���䴴����
	private ArrayList<Client> mClients = new ArrayList<>();		//�����ڵ������û�
	
	public Client getHost() {
		return mHost;
	}
	synchronized public void setHost(Client Host) {
		this.mHost = Host;
		add(mHost);
	}
	
	public int getID() {
		return mID;
	}
	
	synchronized public void add(Client client) {
		mClients.add(client);
	}
	
	
	/**
	 * �Ѹ��·��͸���������������Ա
	 * @param updation
	 * @throws Exception
	 */
	synchronized public void updateAllMember(String updation) throws Exception {
		int size = mClients.size();
		for(int i = 0; i < size; i++) {
			Client client = mClients.get(i); 			
			client.getPrintWriter2().println(updation + "\n" + "$$END$$");
		}
	}
}

