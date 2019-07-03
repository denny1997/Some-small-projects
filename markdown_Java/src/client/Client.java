package client;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;

import javafx.scene.input.Mnemonic;
import sun.security.pkcs.SigningCertificateInfo;
import util.RequestType;
import util.SignInfo;
import util.Utility;

/**
 * ������һ���ͻ����û�
 */
public class Client {
	private static final String SERVER_IP = "127.0.0.1";
	private static final int SERVER_PORT = 8080; 
	private static final int ROOM_SERVER_PORT = 8081;
	
	private static final int CONNECT_TIME_OUT = 15000;	//���ӳ�ʱ����Ϊ15s
	
	private String name;	//�û���
	private int roomID = -1;
	private String IP;		//������������ͻ���һ������ip
	private Socket socket;
	private Socket socket2;
	
	//socket��ȡ�Ķ�������д����
	private InputStream mStreamReader = null;
	private OutputStream mStreamWriter = null;
	
	private PrintWriter mPrintWriter = null;
	
	private BufferedReader mBufferedReader = null;
	
	//���������ݸ��ºͻ�ȡ�Ķ�������д����
	private InputStream mStreamReader2 = null;
	private OutputStream mStreamWriter2 = null;
	
	private PrintWriter mPrintWriter2 = null;
	
	private BufferedReader mBufferedReader2 = null;

	
	public Client() {
		IP = null;
		socket = null;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public String getName() {
		return name;
	}

	public int getRoomID() {
		return roomID;
	}

	public String getIP() {
		return IP;
	}

	public void setIP(String IP) {
		this.IP = IP;
	}

	public void setRoomID(int room) {
		this.roomID = room;
	}

	public void setSocket(Socket socket) throws IOException {
		this.socket = socket;
		
		mStreamReader = socket.getInputStream();
		mStreamWriter = socket.getOutputStream();
		
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
	
	public void setSocket2(Socket socket) throws IOException {
		this.socket2 = socket;
		
		mStreamReader2 = socket.getInputStream();
		mStreamWriter2 = socket.getOutputStream();
		
		mPrintWriter2 = 
			new PrintWriter(
				new BufferedWriter(
					new OutputStreamWriter(
						mStreamWriter2)), true);
			
		mBufferedReader2 = 
			new BufferedReader(
				new InputStreamReader(
						mStreamReader2));
	}

	public Socket getSocket() {
		return socket;
	}
	public Socket getSocket2() {
		return socket2;
	}
	
	public InputStream getReader() {
		return mStreamReader;
	}
	public OutputStream getWriter() {
		return mStreamWriter;
	}
	
	public InputStream getReader2() {
		return mStreamReader2;
	}
	public OutputStream getWriter2() {
		return mStreamWriter2;
	}
	
	/**
	 * ���ӷ�����
	 * @throws UnknownHostException
	 * @throws IOException
	 */
	public void connectServer() throws UnknownHostException, IOException {
		
		socket = new Socket();
		socket.connect(new InetSocketAddress(SERVER_IP, SERVER_PORT), 
						CONNECT_TIME_OUT);
		
		mStreamReader = socket.getInputStream();
		mStreamWriter = socket.getOutputStream();
		
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
	
	/**
	 * ���ӷ���������˿�
	 * @throws UnknownHostException
	 * @throws IOException
	 */
	private void connectServer2() throws UnknownHostException, IOException {
		
		socket2 = new Socket();
		socket2.connect(new InetSocketAddress(SERVER_IP, ROOM_SERVER_PORT), 
						CONNECT_TIME_OUT);
		
		mStreamReader2 = socket2.getInputStream();
		mStreamWriter2 = socket2.getOutputStream();
		
		mPrintWriter2 = 
			new PrintWriter(
				new BufferedWriter(
					new OutputStreamWriter(
						mStreamWriter2)), true);
			
		mBufferedReader2 = 
			new BufferedReader(
				new InputStreamReader(
						mStreamReader2));
	}
	
	
	public PrintWriter getPrintWriter() {
		return mPrintWriter;
	}
	public BufferedReader getBufferedReader() {
		return mBufferedReader;
	}
	
	public PrintWriter getPrintWriter2() {
		return mPrintWriter2;
	}
	public BufferedReader getBufferedReader2() {
		return mBufferedReader2;
	}
	
	/**
	 * ��������ύ���������ʽΪ��[��������]#[�����б�(�м���#����)]
	 * @param request
	 */
	public void disposeRequest(RequestType type, String ... args) throws Exception {
		//��������
		switch (type) {
		case LOGIN:
			disposeLogin(type);
			break;
			
		case CUT_CONNECT:
			disposeCut(type);
			break;
			
		case CREATE_ROOM:
			disposeCreateRoom(type);
			break;
			
		case JOIN_ROOM:
			disposeJoinRoom(type, args[0]);
			break;
			
		case UPLOAD_UPDATION:
			disposeUpdation(type, args[0]);
			break;
			
		default:
			break;
		}
	}
	
	
	/**
	 * �����¼��ע��
	 * @throws Exception
	 */
	private void disposeLogin(RequestType type) throws Exception {
		//��������ύ����
		String request = type + "#";
		mPrintWriter.println(request);
		
		//���ջظ�
		String respond = mBufferedReader.readLine();
		String[] s = respond.split("#");
		
		if(s[0].equals(SignInfo.SUCCESS.toString())) {
			System.out.println("yes");
			this.name = s[1];
			this.IP = s[1];
		}
		else throw new Exception(respond);
	}
	
	
	/**
	 * �Ͽ��������������
	 * @param type
	 * @throws Exception
	 */
	private void disposeCut(RequestType type) throws Exception {
		String request = type + "#";
		mPrintWriter.println(request);
	}
	
	
	/**
	 * ���󴴽�����
	 * @param type
	 * @throws Exception
	 */
	private void disposeCreateRoom(RequestType type) throws Exception {
		String request = type + "#";
		mPrintWriter.println(request);
		
		//���ջظ�
		String respond = mBufferedReader.readLine();
		String[] s = respond.split("#");
				
		if(!s[0].equals(SignInfo.SUCCESS.toString()))
			throw new Exception(respond);
		
		int id = Integer.parseInt(s[1]);
		
		try {
			connectServer2();
			
			request = RequestType.CONNET_ROOM_SERVER + "#" + name;
			mPrintWriter2.println(request);
			
			respond = mBufferedReader2.readLine();
			s = respond.split("#");
			
			if(s[0].equals(SignInfo.SUCCESS.toString())) {
				this.roomID = id;
			}
			else throw new Exception();
		} catch(Exception e) {
			e.printStackTrace();
			throw new Exception("���ӷ��������ʧ�ܣ�");
		}
	}
	
	
	/**
	 * ���뷿��
	 * @param type
	 * @param id
	 * @throws Exception
	 */
	private void disposeJoinRoom(RequestType type, String id) throws Exception {
		String request = type + "#" + id;
		mPrintWriter.println(request);
		
		//���ջظ�
		String respond = mBufferedReader.readLine();
		String[] s = respond.split("#");
				
		if(s[0].equals(SignInfo.SUCCESS.toString())) {
			this.roomID = Integer.parseInt(id);
		}
		else throw new Exception(respond);
		
		try {
			connectServer2();
			
			request = RequestType.CONNET_ROOM_SERVER + "#" + name;
			mPrintWriter2.println(request);
			
			respond = mBufferedReader2.readLine();
			s = respond.split("#");
			
			if(s[0].equals(SignInfo.SUCCESS.toString())) {
				this.roomID = Integer.parseInt(id);
			}
			else throw new Exception();
		} catch(Exception e) {
			e.printStackTrace();
			throw new Exception("���ӷ��������ʧ�ܣ�");
		}
		
	}
	
	
	private void disposeUpdation(RequestType type, String updation) throws Exception {
		String request = type + "#" + roomID + "\n" + updation + "\n" + Utility.END_MARK;
		mPrintWriter2.println(request);
	}
	
	
	/**
	 * �ڼ��뷿��󣬾�һֱ���������Ƿ������µ�����
	 * @return
	 * @throws Exception
	 */
	public String startMonitor_getUpdation() throws Exception {
		return Utility.getCompleteString(mBufferedReader2);
	}
}







