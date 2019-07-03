package internet;

import java.util.ArrayList;

import client.Client;

/**
 * �û����������������ڹ����������ӵ��û�
 */
public class ClientManager {
	//�����еĿͻ��˶���ӵ����mClients�н���ͳһ����
	private ArrayList<Client> mClients = new ArrayList<>();

	public ClientManager() {}
	
	/**
	 * ��������ӵ��û�
	 * @param client
	 */
	synchronized void add(Client client) {
		mClients.add(client);
	}

	/**
	 * �û��Ͽ�����
	 * @param client
	 */
	synchronized void delete(Client client) {
		mClients.remove(client);
	}

	synchronized int size() {
		return mClients.size();
	}

	synchronized Client get(int n) {
		return (Client) mClients.get(n);
	}
}












