package internet;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.lang.Thread.UncaughtExceptionHandler;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;

import javax.management.loading.MLet;

import org.apache.commons.logging.Log;
import org.docx4j.fonts.microsoft.MicrosoftFonts.Font.Bold;

import com.sun.org.apache.xml.internal.resolver.helpers.Debug;

import client.Client;
import client.Room;
import util.RequestType;
import util.SignInfo;
import util.Utility;

/**
 * Markdown editor ��������������
 */
public class MainServer {
	
	private static final int SERVER_PORT = 8080; 
	private static final int ROOM_SERVER_PORT = 8081;
	
	private static final int MAX_ROOM_NUMBER = 100;
	
	private ServerSocket mServerSocket;
	private ServerSocket mServerSocket2;
	
	private HashMap<String, Client> mClients = new HashMap<>();
	
	private int ipCount = 1000;	//����ip
	
	private HashMap<String, String> mPasswords = new HashMap<>();	//��������
	
	private HashMap<Integer, Room> mRooms = new HashMap<>();	//�������еķ���
	
	public static void main(String[] args)
	{
		MainServer server = new MainServer();	//��ʼ������������
		System.out.println("Main server is starting!");
	}
	
	public MainServer() {
		setServer();
	}
	
	
	/**
	 * ����������
	 */
	private void setServer() {
		new Thread(() -> {
			try {
				mServerSocket = new ServerSocket(SERVER_PORT);
				
			} catch (IOException e) {
				e.printStackTrace();
				Utility.error("���ط���������ʧ�ܣ�");
			}
			
			//���������Ͻ��տͻ��˷�������������Ȼ��������
			try {
				while(true) {
					//���µĿͻ��˷�����������
					Socket socket = mServerSocket.accept();
					
					Client client = new Client();
					client.setSocket(socket);
					
					//�¿�һ���߳�
					new DisposeThread(client, 1).start();
				}
			} catch(Exception e) {
				e.printStackTrace();
				Utility.error("���ӳ�ʱ����ͨ·���ִ���");
			}
		}).start();
		
		new Thread(() -> {
			try {
				mServerSocket2 = new ServerSocket(ROOM_SERVER_PORT);
				
			} catch(IOException e) {
				e.printStackTrace();
				Utility.error("�������������ʧ�ܣ�");
			}
			
			try {
				while(true) {
					//���µĿͻ��˷�����������
					Socket socket = mServerSocket2.accept();
					
					Client client = new Client();
					client.setSocket2(socket);
					
					//�¿�һ���߳�
					new DisposeThread(client, 2).start();
				}
			} catch(Exception e) {
				e.printStackTrace();
				Utility.error("���ӳ�ʱ����ͨ·���ִ���");
			}
		}).start();
	}
	
	
	/**
	 * ������Ϊÿһ���ͻ��˿�һ���µ��߳�������������������
	 */
	public class DisposeThread extends Thread {
		
		private Client mClient;
		private Socket mSocket;	//��ͻ��˽������ӵ�socket
		
		private InputStream mStreamReader = null;
		private OutputStream mStreamWriter = null;
		
		private PrintWriter mPrintWriter = null;
		
		private BufferedReader mBufferedReader = null;
		
		public DisposeThread(Client client, int flag) throws IOException {
			mClient = client;
			if(flag == 1) {
				mSocket = client.getSocket();
			}
			else {
				mSocket = client.getSocket2();
			}
			
			mStreamReader = mSocket.getInputStream();
			mStreamWriter = mSocket.getOutputStream();
			
			mPrintWriter = 
				new PrintWriter(
					new BufferedWriter(
						new OutputStreamWriter(
							mStreamWriter)), true);
				
			mBufferedReader = 
				new BufferedReader(
					new InputStreamReader(
							mStreamReader));
		}
		
		
		public void run() {
			try {
				String request;	//�ӿͻ��˷���������
				boolean exit = false;
				
				while(!exit) {
					request = mBufferedReader.readLine();
					
					String[] s = request.split("#");
					RequestType type = RequestType.valueOf(s[0]);
					
					switch (type) {
					//�ͻ�������Ͽ�����
					case CUT_CONNECT:
						exit = true;						
						break;
						
					//�����¼
					case LOGIN:						
						disposeLogin();
						break;											
						
					case CREATE_ROOM:
						disposeCreateRoom();
						break;
						
					case JOIN_ROOM:
						disposeJoinRoom(s[1]);
						break;
						
					case CONNET_ROOM_SERVER:
						disposeConnectRoom(s[1], mSocket);
						break;
						
					case UPLOAD_UPDATION:
						disposeUpdate(Utility.getCompleteString(mBufferedReader),
										Integer.parseInt(s[1]));
						break;

					default:
						break;
					}
					
				} 
				
				//�˳�����ʱ���Ѷ˿ں����ر�,�ѿͻ��ӱ���ɾ��
				mClients.remove(mClient);
				mPrintWriter.close();
				mBufferedReader.close();
				mSocket.close();
			} 
			catch (Exception e) {
				e.printStackTrace();
				Utility.error("��ͻ��˵����ӳ�����һЩ���⣡");
				return;
			}
		}
		
		
		/**
		 * �����¼����
		 * @param name
		 * @param password
		 * @throws Exception
		 */
		private void disposeLogin() throws Exception {
			int ip = ipCount++;
			mPrintWriter.println(SignInfo.SUCCESS + "#" + ip);
			mClient.setIP(Integer.toString(ip));
			mClient.setName(Integer.toString(ip));			
			mClients.put(Integer.toString(ip), mClient);			
		}
		
		/**
		 * ��������������
		 * @throws Exception
		 */
		private void disposeCreateRoom() throws Exception {
			if(mRooms.size() == MAX_ROOM_NUMBER) {
				mPrintWriter.println("�ܱ�Ǹ����������ӵ�������ܴ������䣡");
			}
			else {
				int id = 0;
				//�ҵ���һ���յ�λ�ã�����һ������id
				for( ; id < MAX_ROOM_NUMBER; id++) {
					if(mRooms.get(id) == null)
						break;
				}
				
				Room room = new Room(id);
				room.setHost(mClient);
				mClient.setRoomID(id);
				mRooms.put(id, room);
				
				mPrintWriter.println(SignInfo.SUCCESS + "#" + id);
			}
		}
		
		
		/**
		 * ������뷿������
		 * @param id Ҫ����ķ����id
		 * @throws Exception
		 */
		private void disposeJoinRoom(String idString) throws Exception {
			int id;
			try {
				id = Integer.parseInt(idString);
			} catch(NumberFormatException e) {
				mPrintWriter.println("����id������0~99�����֣�");
				return;
			}
			Room room = mRooms.get(id);
			if(room == null) {
				mPrintWriter.println("��Ҫ����ķ��䲻���ڣ�");
			}
			else {
				room.add(mClient);
				mClient.setRoomID(id);
				
				mPrintWriter.println(SignInfo.SUCCESS + "#");
			}
		}
		
		
		synchronized private void disposeConnectRoom(String name, Socket socket) throws Exception {
			Client client = mClients.get(name);
			client.setSocket2(socket);
			
			client.getPrintWriter2().println(SignInfo.SUCCESS + "#");
		}
		
		/**
		 * �����·��͵���������������
		 * @param updation
		 * @throws Exception
		 */
		private void disposeUpdate(String updation, int roomID) throws Exception {
			Room room = mRooms.get(roomID);
			
			room.updateAllMember(updation);
		}
	}

}







