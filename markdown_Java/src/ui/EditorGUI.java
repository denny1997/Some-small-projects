package ui;

import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.TextArea;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.TextEvent;
import java.awt.event.TextListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.image.TileObserver;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JEditorPane;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.text.html.StyleSheet;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import org.docx4j.dml.Theme;

import client.Client;
import internet.MainServer;
import util.*;

/**
 * ����markdown�༭���Ľ���
 *
 */
public class EditorGUI extends MouseAdapter implements ActionListener, 
DocumentListener, TreeSelectionListener {
	
	private Parser mParser = new Parser();
	
	private HtmlConverter mHtmlConverter = new HtmlConverter();
	
	private JFrame mFrame;
	private JTextArea mTextArea;
	private JEditorPane mEditorPane;
	
	private String mText = "";
	private String mHTML = "";
	private String mCSS = "";
	private boolean mTextChanged = true;
	
	private StyleSheet mStyleSheet;
	
	//�˵�������
	private JMenuBar mMenuBar;
	private JMenu mFileMenu, mClientMenu, mRoomMenu;
	private JMenuItem mOpenItem, mSaveItem, mExportHTMLItem, mExportDocItem, mExportPdfItem, mCreateRoomItem, mJoinRoomItem, mExitRoomItem;
	private JMenuItem mLoginItem;
	private JMenuItem mLogoutItem;
	
	//������
	private DefaultTreeModel mTreeModel;
	private JTree mTree;
	private DefaultMutableTreeNode mRoot;
	
	//�ͻ���
	private Client mClient = null;
	
	//�ж��Ƿ��Ǳ��˵ĸ���
	private boolean isOther;
	
	public static void main(String[] args) {
		try {
            UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
        } catch (Exception e) {
        }
		EditorGUI editor = new EditorGUI();
		editor.show();			
	}
	
	public EditorGUI() {
		setFrame();
		setMenu();		
	}
	
	/**
	 * ����UI
	 */
	private void update() {
		mTextChanged = true;
		mText = mTextArea.getText();
		
		try {
			mHTML = mParser.parseMarkdownToHTML(mText);
			mEditorPane.setText(mHTML);
		} catch(Exception e) {
			e.printStackTrace();
		}

		setTitles();
		
		System.out.println("updated");
	}
	
	/**
	 * ΪĿ¼���±���
	 */
	private void setTitles() {
		Pattern pattern = Pattern.compile("<h(\\d)>(.*?)</h(\\d)>", Pattern.CANON_EQ);
		Matcher matcher = pattern.matcher(mHTML);
		
		mRoot.removeAllChildren();
		while(matcher.find()) {
			int rank = matcher.group(1).charAt(0) - '0';
			String title = matcher.group(2);
			
			DefaultMutableTreeNode target = mRoot;
			for(int i = 1; i < rank; i++) {
				target = (DefaultMutableTreeNode)target.getChildAt(target.getChildCount() - 1);
			}
			mTreeModel.insertNodeInto(new DefaultMutableTreeNode(title), target, target.getChildCount());
		}
		mTree.updateUI();
	}
	
	/**
	 * ����һ��{@link JTextArea}�������༭markdown�ı�
	 */
	private void createTextArea() {
		mTextArea = new JTextArea();
		mTextArea.setLineWrap(true);
		Font font = new Font("Microsoft YaHei", Font.PLAIN, 18);
		mTextArea.setFont(font);
		mTextArea.getDocument().addDocumentListener(this);
		mTextArea.addMouseListener(this);
	}
	
	/**
	 * ����һ��{@link JEditorPane}��,������ʾHTML
	 */
	private void createEditorPane() {
		mEditorPane = new JEditorPane();
		mEditorPane.setContentType("text/html");
		mEditorPane.setEditable(false);
		
		HTMLEditorKit ed = new HTMLEditorKit();
		mEditorPane.setEditorKit(ed);
		
		mStyleSheet = ed.getStyleSheet();
		mStyleSheet.addRule("body {font-family:\"Microsoft YaHei\", Monaco}");
		mStyleSheet.addRule("p {font-size: 14px}");
		
		try {
			mHTML = mParser.parseMarkdownToHTML(mText);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * ����һ��{@link JTree}��������ʾ�ı�������Ŀ¼�ṹ
	 */
	private void createNavigation() {
		mRoot = new DefaultMutableTreeNode("��Ŀ¼");
		mTree = new JTree(mRoot);
		mTree.addTreeSelectionListener(this);
		
		mTreeModel = (DefaultTreeModel)mTree.getModel();
	}
	
	/**
	 * ����һ��{@link JFrame},��Ϊ������
	 */
	private void setFrame() {
		createTextArea();
		createEditorPane();
		createNavigation();
		
		mFrame = new JFrame();
		mFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		mFrame.setLayout(new GridBagLayout());

		{
			JScrollPane s1 = new JScrollPane(mTree);
			GridBagConstraints g1 = new GridBagConstraints();
			g1.gridx = 0;
			g1.gridy = 0;
			g1.weightx = 0;
			g1.weighty = 1;
			g1.ipadx = 150;
			g1.fill = GridBagConstraints.VERTICAL;
			mFrame.add(s1, g1);
		}
		
		{
			JScrollPane s2 = new JScrollPane(mTextArea);
			GridBagConstraints g2 = new GridBagConstraints();
			g2.gridx = 1;
			g2.gridy = 0;
			g2.weightx = 1;
			g2.weighty = 1;
			g2.ipadx = 250;
			g2.fill = GridBagConstraints.BOTH;
			mFrame.add(s2, g2);
		}
		
		{
			JScrollPane s3 = new JScrollPane(mEditorPane);
			GridBagConstraints g3 = new GridBagConstraints();
			g3.gridx = 2;
			g3.gridy = 0;
			g3.weightx = 1;
			g3.weighty = 1;
			g3.ipadx = 250;
			g3.fill = GridBagConstraints.BOTH;
			mFrame.add(s3, g3);
		}
		
		mFrame.setTitle("Markdown Editor : ����ģʽ");
		mFrame.setSize(800, 600);
		
		mFrame.addWindowListener(new WindowAdapter() {
			
			@Override
			public void windowClosing(WindowEvent e) {
				if(mClient != null) {
					try {
						mClient.disposeRequest(RequestType.CUT_CONNECT);
					} catch (Exception e1) {
						e1.printStackTrace();
						Utility.error("�޷��رտͻ��˶˿ڣ�");
					}
				}
			}
		});
	}
	
	private void setMenu() {
		mMenuBar = new JMenuBar();
		mFrame.setJMenuBar(mMenuBar);
		
		//�ļ��˵�
		{
			mFileMenu = new JMenu("�ļ�");
			mFileMenu.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mMenuBar.add(mFileMenu);
			
			mOpenItem = new JMenuItem("��");
			mOpenItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mOpenItem);
			mOpenItem.addActionListener(this);
			
			mSaveItem = new JMenuItem("����");
			mSaveItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mSaveItem);
			mSaveItem.addActionListener(this);
			
			mExportHTMLItem = new JMenuItem("����HTML");
			mExportHTMLItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mExportHTMLItem);
			mExportHTMLItem.addActionListener(this);
			
			mExportDocItem = new JMenuItem("����docx");
			mExportDocItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mExportDocItem);
			mExportDocItem.addActionListener(this);
						
		}
		
		//��¼��ע��˵�
		{
			mClientMenu = new JMenu("��¼/ע��");
			mClientMenu.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mMenuBar.add(mClientMenu);
			
			mLoginItem = new JMenuItem("��¼");
			mLoginItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mClientMenu.add(mLoginItem);
			mLoginItem.addActionListener(this);
			
			mLogoutItem = new JMenuItem("ע��");
			mLogoutItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mClientMenu.add(mLogoutItem);
			mLogoutItem.addActionListener(this);
		}
				
		{
			mRoomMenu = new JMenu("����");
			mRoomMenu.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mMenuBar.add(mRoomMenu);
			
			mCreateRoomItem = new JMenuItem("��������");
			mCreateRoomItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mRoomMenu.add(mCreateRoomItem);
			mCreateRoomItem.addActionListener(this);
			
			mJoinRoomItem = new JMenuItem("���뷿��");
			mJoinRoomItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mRoomMenu.add(mJoinRoomItem);
			mJoinRoomItem.addActionListener(this);			
			
			mRoomMenu.setVisible(false);
		}
	}
	
	/**
	 * ��ʾ�ý���
	 */
	public void show() {
		mFrame.setVisible(true);
	}
	
	
	//�����¼�
	@Override
	public void actionPerformed(ActionEvent event) {
		Object item = event.getSource();
		
		//��markdown�ļ�
	    if(item == mOpenItem) {
	    	String tmp = Utility.getContentFromExternalFile();
	    	if(tmp != null) {
	    		mText = tmp;
		    	mTextArea.setText(mText);
	    	}
	    }
	    
	    //����
	    else if(item == mSaveItem) {
	    	if(mTextChanged) {
	    		if(Utility.saveContent(mText, "md"))
	    			mTextChanged = false;
	    	}
	    }
		
		//����HTML
	    else if(item == mExportHTMLItem) {
	    	Utility.saveContent(Utility.getStyledHTML(mHTML, mCSS), "html");
		}
	    
	    //����docx
	    else if(item == mExportDocItem) {
	    	try {
				mHtmlConverter.saveHtmlToDocx(Utility.getStyledHTML(mHTML, mCSS));
			} catch (Exception e) {
				System.out.println("����docxʧ�ܣ�");
			}
	    }
	      
	    //��¼
	    else if(item == mLoginItem) {
	    	if(mClient != null) {
	    		Utility.info("���Ѿ���¼��");
	    	} else
				try {
					login();
				} catch (Exception e) {
					e.printStackTrace();
				}
	    }
	    
	    //ע��
	    else if(item == mLogoutItem) {
	    	if(mClient == null) {
	    		Utility.info("�㻹δ��¼��");
	    	} else
				try {
					logout();
				} catch (Exception e) {
					e.printStackTrace();
				}
	    }
	    
	    //��������
	    else if(item == mCreateRoomItem) {
	    	if(mClient.getRoomID() != -1) {
	    		Utility.info("�����ڷ����ڣ�");
	    	}
	    	else createRoom();
	    }
	    
	    //���뷿��
	    else if(item == mJoinRoomItem) {
	    	if(mClient.getRoomID() != -1) {
	    		Utility.info("�����ڷ����ڣ�");
	    	}
	    	else joinRoom();
	    }
	    
	}
	
	private Thread lastThread = null;		//�洢��һ��Ҫ����UI���̣߳�����һ������
	private void updateUIEfficiently() {
		new Thread(() -> {
			Thread last = lastThread;
			lastThread = Thread.currentThread();
			
			try {
				//������һ�����µ��߳�
				if(last != null) {
					last.interrupt();
				}
				Thread.sleep(1000);
			} catch(InterruptedException exc) {
				return;
			}

			if(Thread.currentThread().isInterrupted()) return;
			SwingUtilities.invokeLater(() -> {update();});
			
			if(isOnline&&(!isOther)) {
				String updation = mTextArea.getText();
				try {
					mClient.disposeRequest(RequestType.UPLOAD_UPDATION, updation);
				} catch (Exception e) {
					e.printStackTrace();
					Utility.error("������������ӳ��ִ���");
				}
			}
			isOther=false;		
		}).start();
	}
	
	
	@Override
	public void insertUpdate(DocumentEvent e) {
		updateUIEfficiently();
	}

	@Override
	public void removeUpdate(DocumentEvent e) {
		updateUIEfficiently();
	}

	@Override
	public void changedUpdate(DocumentEvent e) {

	}
	
	//�����λ��
	@Override
	public void mouseClicked(MouseEvent e) {
		Object item = e.getSource();
		
		if(item == mTextArea) {
			int position = mTextArea.getCaretPosition();
		}
			
	}

	//��������ѡ�е��¼�
	@Override
	public void valueChanged(TreeSelectionEvent e) {
		DefaultMutableTreeNode selectedNode=(DefaultMutableTreeNode) 
				mTree.getLastSelectedPathComponent();//�������ѡ���Ľڵ�  
		
		String title = selectedNode.toString();
		int level = selectedNode.getLevel();
		System.out.println(level);
		StringBuilder sb = new StringBuilder();
		for(int i = 0; i < level; i++)
			sb.append("#");
		sb.append(title);
		
		int pos = mText.indexOf(sb.toString());
		mTextArea.setSelectionStart(pos);
		mTextArea.setSelectionEnd(pos);
	}
	
	private void login() throws Exception{		
	    	mClient = new Client();   		
	    	try {
	    		mClient.connectServer();
	    	} catch(Exception e) {
	    		e.printStackTrace();
	    		Utility.error("���ӷ�����ʧ�ܣ�");
	    		mClient=null;
	    		return;
	    	}
	    	
	    	mClient.disposeRequest(RequestType.LOGIN);
			mFrame.setTitle("Markdown Editor : ����ģʽ" );	
			mRoomMenu.setVisible(true);
	}
	
	private void logout() throws Exception{		   	
		
    	mClient.disposeRequest(RequestType.CUT_CONNECT);
    	mClient=null;
    	isOnline=false;
		mFrame.setTitle("Markdown Editor : ����ģʽ" );	
		mRoomMenu.setVisible(false);
}
	
	private boolean isOnline;
	
	/**
	 * ������һ������󣬾�Ҫ��ʼһֱ����������������Ϣ
	 */
	private void startUpdateMonitor() {
		new Thread(() -> {
			try {
				while(true) {
					String updation = mClient.startMonitor_getUpdation();
					isOther=true;
					SwingUtilities.invokeLater(() -> {
						mTextArea.setText(updation);
					});
				}
			} catch(Exception e) {
				e.printStackTrace();
				Utility.error("������������жϣ�");
				return;
			}
		}).start();
	}
	
	/**
	 * ��������
	 */
	private void createRoom() {
		new Thread(() -> {
			try {
				mClient.disposeRequest(RequestType.CREATE_ROOM);
			} catch (Exception e) {
				e.printStackTrace();
				Utility.error(e.getMessage());
				return;
			}
			
			Utility.info("���䴴���ɹ�!���Ѵ������䣺 " + mClient.getRoomID());
			isOnline = true;
			SwingUtilities.invokeLater(() -> {
				mFrame.setTitle(mFrame.getTitle() + "(�����ڷ��䣺 " + mClient.getRoomID() + ")");
			});
			
			startUpdateMonitor();
		}).start();
	}
	
	/**
	 * ���뷿��
	 */
	private void joinRoom() {
		new Thread(() -> {
			String idString = JOptionPane.showInputDialog("��������Ҫ����ķ���id��");
			
			try {
				mClient.disposeRequest(RequestType.JOIN_ROOM, idString);
			} catch(Exception e) {
				e.printStackTrace();
				Utility.error(e.getMessage());
				return;
			}
			
			Utility.info("�������ɹ������Ѽ��뷿�䣺 " + mClient.getRoomID());
			isOnline = true;
			SwingUtilities.invokeLater(() -> {
				mFrame.setTitle(mFrame.getTitle() + "(�����ڷ��䣺 " + mClient.getRoomID() + ")");
				mTextArea.setEditable(true);
			});
			
			startUpdateMonitor();
		}).start();
	}
}









