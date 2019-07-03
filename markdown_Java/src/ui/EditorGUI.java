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
 * 定义markdown编辑器的界面
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
	
	//菜单栏内容
	private JMenuBar mMenuBar;
	private JMenu mFileMenu, mClientMenu, mRoomMenu;
	private JMenuItem mOpenItem, mSaveItem, mExportHTMLItem, mExportDocItem, mExportPdfItem, mCreateRoomItem, mJoinRoomItem, mExitRoomItem;
	private JMenuItem mLoginItem;
	private JMenuItem mLogoutItem;
	
	//导航栏
	private DefaultTreeModel mTreeModel;
	private JTree mTree;
	private DefaultMutableTreeNode mRoot;
	
	//客户端
	private Client mClient = null;
	
	//判断是否是别人的更新
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
	 * 更新UI
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
	 * 为目录更新标题
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
	 * 创建一个{@link JTextArea}域，用来编辑markdown文本
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
	 * 创建一个{@link JEditorPane}域,用来显示HTML
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
	 * 创建一个{@link JTree}域，用来显示文本的树形目录结构
	 */
	private void createNavigation() {
		mRoot = new DefaultMutableTreeNode("根目录");
		mTree = new JTree(mRoot);
		mTree.addTreeSelectionListener(this);
		
		mTreeModel = (DefaultTreeModel)mTree.getModel();
	}
	
	/**
	 * 创建一个{@link JFrame},作为主界面
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
		
		mFrame.setTitle("Markdown Editor : 离线模式");
		mFrame.setSize(800, 600);
		
		mFrame.addWindowListener(new WindowAdapter() {
			
			@Override
			public void windowClosing(WindowEvent e) {
				if(mClient != null) {
					try {
						mClient.disposeRequest(RequestType.CUT_CONNECT);
					} catch (Exception e1) {
						e1.printStackTrace();
						Utility.error("无法关闭客户端端口？");
					}
				}
			}
		});
	}
	
	private void setMenu() {
		mMenuBar = new JMenuBar();
		mFrame.setJMenuBar(mMenuBar);
		
		//文件菜单
		{
			mFileMenu = new JMenu("文件");
			mFileMenu.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mMenuBar.add(mFileMenu);
			
			mOpenItem = new JMenuItem("打开");
			mOpenItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mOpenItem);
			mOpenItem.addActionListener(this);
			
			mSaveItem = new JMenuItem("保存");
			mSaveItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mSaveItem);
			mSaveItem.addActionListener(this);
			
			mExportHTMLItem = new JMenuItem("导出HTML");
			mExportHTMLItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mExportHTMLItem);
			mExportHTMLItem.addActionListener(this);
			
			mExportDocItem = new JMenuItem("导出docx");
			mExportDocItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mFileMenu.add(mExportDocItem);
			mExportDocItem.addActionListener(this);
						
		}
		
		//登录和注册菜单
		{
			mClientMenu = new JMenu("登录/注销");
			mClientMenu.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mMenuBar.add(mClientMenu);
			
			mLoginItem = new JMenuItem("登录");
			mLoginItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mClientMenu.add(mLoginItem);
			mLoginItem.addActionListener(this);
			
			mLogoutItem = new JMenuItem("注销");
			mLogoutItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mClientMenu.add(mLogoutItem);
			mLogoutItem.addActionListener(this);
		}
				
		{
			mRoomMenu = new JMenu("房间");
			mRoomMenu.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mMenuBar.add(mRoomMenu);
			
			mCreateRoomItem = new JMenuItem("创建房间");
			mCreateRoomItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mRoomMenu.add(mCreateRoomItem);
			mCreateRoomItem.addActionListener(this);
			
			mJoinRoomItem = new JMenuItem("加入房间");
			mJoinRoomItem.setFont(new Font("Microsoft YaHei", Font.PLAIN, 15));
			mRoomMenu.add(mJoinRoomItem);
			mJoinRoomItem.addActionListener(this);			
			
			mRoomMenu.setVisible(false);
		}
	}
	
	/**
	 * 显示该界面
	 */
	public void show() {
		mFrame.setVisible(true);
	}
	
	
	//监听事件
	@Override
	public void actionPerformed(ActionEvent event) {
		Object item = event.getSource();
		
		//打开markdown文件
	    if(item == mOpenItem) {
	    	String tmp = Utility.getContentFromExternalFile();
	    	if(tmp != null) {
	    		mText = tmp;
		    	mTextArea.setText(mText);
	    	}
	    }
	    
	    //保存
	    else if(item == mSaveItem) {
	    	if(mTextChanged) {
	    		if(Utility.saveContent(mText, "md"))
	    			mTextChanged = false;
	    	}
	    }
		
		//导出HTML
	    else if(item == mExportHTMLItem) {
	    	Utility.saveContent(Utility.getStyledHTML(mHTML, mCSS), "html");
		}
	    
	    //导出docx
	    else if(item == mExportDocItem) {
	    	try {
				mHtmlConverter.saveHtmlToDocx(Utility.getStyledHTML(mHTML, mCSS));
			} catch (Exception e) {
				System.out.println("保存docx失败！");
			}
	    }
	      
	    //登录
	    else if(item == mLoginItem) {
	    	if(mClient != null) {
	    		Utility.info("你已经登录！");
	    	} else
				try {
					login();
				} catch (Exception e) {
					e.printStackTrace();
				}
	    }
	    
	    //注销
	    else if(item == mLogoutItem) {
	    	if(mClient == null) {
	    		Utility.info("你还未登录！");
	    	} else
				try {
					logout();
				} catch (Exception e) {
					e.printStackTrace();
				}
	    }
	    
	    //创建房间
	    else if(item == mCreateRoomItem) {
	    	if(mClient.getRoomID() != -1) {
	    		Utility.info("你已在房间内！");
	    	}
	    	else createRoom();
	    }
	    
	    //加入房间
	    else if(item == mJoinRoomItem) {
	    	if(mClient.getRoomID() != -1) {
	    		Utility.info("你已在房间内！");
	    	}
	    	else joinRoom();
	    }
	    
	}
	
	private Thread lastThread = null;		//存储上一个要更新UI的线程，供下一个阻塞
	private void updateUIEfficiently() {
		new Thread(() -> {
			Thread last = lastThread;
			lastThread = Thread.currentThread();
			
			try {
				//阻塞上一个更新的线程
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
					Utility.error("与服务器端连接出现错误！");
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
	
	//检测光标位置
	@Override
	public void mouseClicked(MouseEvent e) {
		Object item = e.getSource();
		
		if(item == mTextArea) {
			int position = mTextArea.getCaretPosition();
		}
			
	}

	//导航栏树选中的事件
	@Override
	public void valueChanged(TreeSelectionEvent e) {
		DefaultMutableTreeNode selectedNode=(DefaultMutableTreeNode) 
				mTree.getLastSelectedPathComponent();//返回最后选定的节点  
		
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
	    		Utility.error("连接服务器失败！");
	    		mClient=null;
	    		return;
	    	}
	    	
	    	mClient.disposeRequest(RequestType.LOGIN);
			mFrame.setTitle("Markdown Editor : 在线模式" );	
			mRoomMenu.setVisible(true);
	}
	
	private void logout() throws Exception{		   	
		
    	mClient.disposeRequest(RequestType.CUT_CONNECT);
    	mClient=null;
    	isOnline=false;
		mFrame.setTitle("Markdown Editor : 离线模式" );	
		mRoomMenu.setVisible(false);
}
	
	private boolean isOnline;
	
	/**
	 * 当加入一个房间后，就要开始一直监测服务器发来的信息
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
				Utility.error("与服务器连接中断！");
				return;
			}
		}).start();
	}
	
	/**
	 * 创建房间
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
			
			Utility.info("房间创建成功!你已创建房间： " + mClient.getRoomID());
			isOnline = true;
			SwingUtilities.invokeLater(() -> {
				mFrame.setTitle(mFrame.getTitle() + "(你已在房间： " + mClient.getRoomID() + ")");
			});
			
			startUpdateMonitor();
		}).start();
	}
	
	/**
	 * 加入房间
	 */
	private void joinRoom() {
		new Thread(() -> {
			String idString = JOptionPane.showInputDialog("请输入你要加入的房间id：");
			
			try {
				mClient.disposeRequest(RequestType.JOIN_ROOM, idString);
			} catch(Exception e) {
				e.printStackTrace();
				Utility.error(e.getMessage());
				return;
			}
			
			Utility.info("房间加入成功！你已加入房间： " + mClient.getRoomID());
			isOnline = true;
			SwingUtilities.invokeLater(() -> {
				mFrame.setTitle(mFrame.getTitle() + "(你已在房间： " + mClient.getRoomID() + ")");
				mTextArea.setEditable(true);
			});
			
			startUpdateMonitor();
		}).start();
	}
}









