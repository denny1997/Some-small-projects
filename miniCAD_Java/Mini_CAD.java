package miniCAD;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
//���廭ͼ�Ļ���ͼ�ε�Ԫ
public class Mini_CAD extends JFrame //���࣬��չ��JFrame�࣬��������������
{
    private ObjectInputStream input;
    private ObjectOutputStream output; //����������������������úͱ���ͼ���ļ�
    private JButton choices[];         //��ť���飬����������ƵĹ��ܰ�ť
    private String names[] = {       
        "Pencil", //Ǧ�ʻ���Ҳ����������϶��������ͼ
        "Line", //����ֱ��
        "Rect", //���ƿ��ľ���
        "fRect", //������ָ����ɫ����ʵ�ľ���
        "Oval", //���ƿ�����Բ
        "fOval", //������ָ����ɫ����ʵ����Բ
        "Circle", //����Բ��
        "fCircle", //������ָ����ɫ����ʵ��Բ��
        "RoundRect", //���ƿ���Բ�Ǿ���
        "frRect", //������ָ����ɫ����ʵ��Բ�Ǿ���
        "Polyline",//���ƶ������
        "Polygon",//���ƶ����
        "Rubber", //��Ƥ������������ȥ�Ѿ����ƺõ�ͼ��
        "Word"  ,    //�������ְ�ť�������ڻ�ͼ����ʵ����������
        "Size+", 
        "Size-", 
        "Stroke+",
        "Stroke-",
        "Delete",
        "Select"   //ѡ��
    };
    private String tipText[] = {        
        "Draw at will",
        "Draw a straight line",
        "Draw a rectangle",
        "Fill a ractangle",
        "Draw an oval",
        "Fill an oval",
        "Draw a circle",
        "Fill a circle",
        "Draw a round rectangle",
        "Fill a round rectangle",
        "Draw a polyline",
        "Draw a polygon",
        "Erase at will",
        "Write down what u want",
        "Increase size",
        "Decrease size",
        "Increase stroke",
        "Decreae stroke",
        "Delete a component",
        "Select a component"
    };
    JToolBar buttonPanel;              //���尴ť���
    private JLabel statusBar;            //��ʾ���״̬����ʾ��
    private DrawPanel drawingArea;       //��ͼ����
    private int width = 1200,  height = 1000;    //���廭ͼ�����ʼ��С
    drawings[] itemList = new drawings[5000]; //������Ż���ͼ�ε�����
    private int currentChoice = 19;            //����Ĭ�ϻ�ͼ״̬Ϊ��ʻ�
    int index = 0;                         //��ǰ�Ѿ����Ƶ�ͼ����Ŀ
    private Color color = Color.black;     //��ǰ������ɫ
    int R, G, B;                           //������ŵ�ǰɫ��ֵ
    int f1, f2;                  //������ŵ�ǰ������
    private float stroke = 1.0f;  //���û��ʴ�ϸ��Ĭ��ֵΪ1.0f
    JCheckBox bold, italic;      //����������ѡ���
    int cur_x,cur_y;
    public Mini_CAD() //���캯��
    {
        super("Drawing Pad");
        JMenuBar bar = new JMenuBar();      //����˵���
        JMenu fileMenu = new JMenu("File");
        fileMenu.setMnemonic('F');
//�½��ļ��˵���
        JMenuItem newItem = new JMenuItem("New");
        newItem.setMnemonic('N');
        newItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        newFile();      //�����������������½��ļ�������
                    }
                });
        fileMenu.add(newItem);
//�����ļ��˵���
        JMenuItem saveItem = new JMenuItem("Save");
        saveItem.setMnemonic('S');
        saveItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        saveFile();     //���������������ñ����ļ�������
                    }
                });
        fileMenu.add(saveItem);
//���ļ��˵���
        JMenuItem loadItem = new JMenuItem("Load");
        loadItem.setMnemonic('L');
        loadItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        loadFile();     //���������������ô��ļ�������
                    }
                });
        fileMenu.add(loadItem);
        fileMenu.addSeparator();
//�˳��˵���
        JMenuItem exitItem = new JMenuItem("Exit");
        exitItem.setMnemonic('X');
        exitItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        System.exit(0); //��������������˳���ͼ�����
                    }
                });
        fileMenu.add(exitItem);
        bar.add(fileMenu);
//������ɫ�˵���
        JMenu changeMenu = new JMenu("Change");
        changeMenu.setMnemonic('C');
//ѡ����ɫ�˵���
        JMenuItem colorItem = new JMenuItem("Change Color");
        colorItem.setMnemonic('O');
        colorItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        chooseColor();  //����������������ѡ����ɫ������
                    }
                });
        changeMenu.add(colorItem);
        //bar.add(colorMenu);
//����������ϸ�˵���
        JMenuItem strokeItem = new JMenuItem("Change Stroke");
        strokeItem.setMnemonic('K');
        strokeItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        setStroke();
                    }
                });
        changeMenu.add(strokeItem);
        
        JMenuItem textItem = new JMenuItem("Change text");
        textItem.setMnemonic('T');
        textItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        setText();
                    }
                });
        changeMenu.add(textItem);
        bar.add(changeMenu);
//������ʾ�˵���
        JMenu helpMenu = new JMenu("Help");
        helpMenu.setMnemonic('H');
//������ʾ�˵���
        JMenuItem aboutItem = new JMenuItem("About this Drawing Pad!");
        aboutItem.setMnemonic('A');
        aboutItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        JOptionPane.showMessageDialog(null,
                                "This is a mini CAD pad!\nCopyright (c) 2018 Denny Lu ",
                                " Info ",
                                JOptionPane.INFORMATION_MESSAGE);
                    }
                });
        helpMenu.add(aboutItem);
        bar.add(helpMenu);
//�������ֻ���ͼ�εİ�ť
        drawingArea = new DrawPanel();
        choices = new JButton[names.length];
        buttonPanel = new JToolBar(JToolBar.VERTICAL);
        buttonPanel = new JToolBar(JToolBar.HORIZONTAL);
        ButtonHandler handler = new ButtonHandler();
        ButtonHandler1 handler1 = new ButtonHandler1();
        for (int i = 0; i < choices.length; i++) {
        	choices[i] = new JButton(names[i]);
            choices[i].setToolTipText(tipText[i]);
            buttonPanel.add(choices[i]);
        }
//���������������밴ť����
        for (int i = 0; i < choices.length - 7; i++) {
            choices[i].addActionListener(handler);
        }
        for (int i = choices.length-7; i < choices.length-1 ; i++) {
            choices[i].addActionListener(handler1);
        }
        choices[choices.length - 1].addActionListener(handler);

        bold = new JCheckBox("BOLD");
        italic = new JCheckBox("ITALIC");
        checkBoxHandler cHandler = new checkBoxHandler();
        bold.addItemListener(cHandler);
        italic.addItemListener(cHandler);
        buttonPanel.add(bold);
        buttonPanel.add(italic);
        Container c = getContentPane();
        super.setJMenuBar(bar);
        c.add(buttonPanel, BorderLayout.NORTH);
        c.add(drawingArea, BorderLayout.CENTER);
        statusBar = new JLabel();
        c.add(statusBar, BorderLayout.SOUTH);
        statusBar.setText("     Welcome To The Little CAD Pad!!!  :)");
        createNewItem();
        setSize(width, height);
        this.setVisible(true);
    }
//��ť������ButtonHanler�࣬�ڲ��࣬��������������ť�Ĳ���
    public class ButtonHandler implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            for (int j = 0; j < choices.length ; j++) {
                if (e.getSource() == choices[j]) {
                    currentChoice = j;
                    createNewItem();
                    repaint();
                }
            }
        }
    }
//��ť������ButtonHanler1�࣬����������ɫѡ�񡢻��ʴ�ϸ���á��������밴ť�Ĳ���
    public class ButtonHandler1 implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            if (e.getSource() == choices[choices.length - 2]) {
                int i;
                for(i=0; i<index; i++)
                {
                	if(itemList[i].ischoose)
                		itemList[i]=new drawings();
                }
                repaint();
            }
            if (e.getSource() == choices[choices.length - 3])
            {
            	int i;           
            	for(i=0; i<index; i++)
                {
                	if((itemList[i].ischoose)&&(itemList[i].type!=13))
                	{
                		itemList[i].stroke=(float) (itemList[i].stroke*0.95);
                	}
                }
            	repaint();
            }            
            if (e.getSource() == choices[choices.length - 4])
            {
            	int i;           
            	for(i=0; i<index; i++)
                {
                	if((itemList[i].ischoose)&&(itemList[i].type!=13))
                	{
                		itemList[i].stroke=(float) (itemList[i].stroke*1.05);
                	}
                }
            	repaint();
            }            
            if (e.getSource() == choices[choices.length - 5])
            {
            	int i;
                for(i=0; i<index; i++)
                {
                	if(itemList[i].ischoose)
                	{
                		if(itemList[i].type!=13)
                		{
                			itemList[i].dx=(int) (itemList[i].dx*0.95);
            				itemList[i].dy=(int) (itemList[i].dy*0.95);
            				itemList[i].x2=itemList[i].x1+itemList[i].dx;
            				itemList[i].y2=itemList[i].y1+itemList[i].dy;
                			if((itemList[i+1].ischoose)&&(i<index-1))
                			{           
                				itemList[i+1].x1=itemList[i].x2;
                				itemList[i+1].y1=itemList[i].y2;
                			}
                		}
                		else
                		{
                			itemList[i].stroke=(float) (itemList[i].stroke*0.75);
                		}
                	}
                }
                repaint();
            }
            if (e.getSource() == choices[choices.length - 6])
            {
            	int i;
                for(i=0; i<index; i++)
                {
                	if(itemList[i].ischoose)
                	{
                		if(itemList[i].type!=13)
                		{
                			itemList[i].dx=(int) (itemList[i].dx*1.05);
            				itemList[i].dy=(int) (itemList[i].dy*1.05);
            				itemList[i].x2=itemList[i].x1+itemList[i].dx;
            				itemList[i].y2=itemList[i].y1+itemList[i].dy;
                			if((itemList[i+1].ischoose)&&(i<index-1))
                			{           
                				itemList[i+1].x1=itemList[i].x2;
                				itemList[i+1].y1=itemList[i].y2;
                			}
                		}
                		else
                		{
                			itemList[i].stroke=(float) (itemList[i].stroke+1);
                		}
                	}
                }
                repaint();
            }
            if (e.getSource() == choices[choices.length - 7]) {
                JOptionPane.showMessageDialog(null,
                        "Please hit the drawing pad to choose the word input position",
                        "Hint", JOptionPane.INFORMATION_MESSAGE);
                currentChoice = 13;
                createNewItem();
                repaint();
            }
        }
    }
//����¼�mouseA�࣬�̳���MouseAdapter��������������Ӧ�¼�����
    class mouseA extends MouseAdapter {
        public void mousePressed(MouseEvent e) {
            statusBar.setText("     Mouse Pressed @:[" + e.getX() +
                    ", " + e.getY() + "]");//����״̬��ʾ
            if((currentChoice!=10)&&(currentChoice!=11)&&(currentChoice!=19))
            {
            	itemList[index].x1 = itemList[index].x2 = e.getX();
            	itemList[index].y1 = itemList[index].y2 = e.getY();
            }
            //�����ǰѡ���ͼ������ʻ�������Ƥ�������������Ĳ���
            if (currentChoice == 0 || currentChoice == 12) {
                itemList[index].x1 = itemList[index].x2 = e.getX();
                itemList[index].y1 = itemList[index].y2 = e.getY();
                index++;
                createNewItem();
            }
            //�����ǰѡ���ͼ��ʽ�������룬������������
            if (currentChoice == 13) {
                itemList[index].x1 = e.getX();
                itemList[index].y1 = e.getY();
                String input;
                input = JOptionPane.showInputDialog(
                        "Please input the text you want!");
                itemList[index].s1 = input;
                itemList[index].x2 = f1;
                itemList[index].y2 = f2;
                index++;
                createNewItem();
                drawingArea.repaint();
            }
            if(currentChoice==19)
            {
            	cur_x=e.getX();
            	cur_y=e.getY();
            }
        }
        public void mouseReleased(MouseEvent e) {
            statusBar.setText("     Mouse Released @:[" + e.getX() +
                    ", " + e.getY() + "]");
            if((currentChoice!=10)&&(currentChoice!=11)&&(currentChoice!=19))
            {
            	if (currentChoice == 0 || currentChoice == 12) {
                itemList[index].x1 = e.getX();
                itemList[index].y1 = e.getY();
            	}
            	itemList[index].x2 = e.getX();
            	itemList[index].y2 = e.getY();
            	repaint();
            	index++;
            	createNewItem();
            }
            
        }
        @Override
        public void mouseClicked(MouseEvent e)
        {
        	if(currentChoice==10)
        	{
        		if(e.getButton()==MouseEvent.BUTTON3)
        		{
        			itemList[index].x1 = e.getX();
        			itemList[index].y1 = e.getY();
        			itemList[index].x2 = e.getX();
        			itemList[index].y2 = e.getY();  
        			index++;
                	createNewItem();
        		}
        		else if(e.getButton()==MouseEvent.BUTTON1)
        		{        			
        			itemList[index-1].x2 = e.getX();
                	itemList[index-1].y2 = e.getY();                	    
                	itemList[index].x1 = e.getX();
        			itemList[index].y1 = e.getY();
        			itemList[index].x2 = e.getX();
        			itemList[index].y2 = e.getY();
        			repaint();
        			index++;
                	createNewItem();
        		}
        	}
        	if(currentChoice==11)
        	{
        		if(e.getButton()==MouseEvent.BUTTON3)
        		{
        			itemList[index].x1 = e.getX();
        			itemList[index].y1 = e.getY();
        			itemList[index].x2 = e.getX();
        			itemList[index].y2 = e.getY();  
        			itemList[index].init_x = e.getX();
        			itemList[index].init_y = e.getY();
        			itemList[index].end = true;
        			index++;
                	createNewItem();
        		}
        		else if(e.getButton()==MouseEvent.BUTTON1)
        		{        			
        			if(index>=1)
        			{
        				itemList[index-1].x2 = e.getX();
	                	itemList[index-1].y2 = e.getY();
	                	itemList[index-1].end = false;
	                	itemList[index].init_x=itemList[index-1].init_x;
	                	itemList[index].init_y=itemList[index-1].init_y;
	                	itemList[index].x1 = e.getX();
	        			itemList[index].y1 = e.getY();
	        			itemList[index].x2 = e.getX();
	        			itemList[index].y2 = e.getY();
	        			itemList[index].end = true;
	        			repaint();
	        			index++;
	                	createNewItem();
        			}        			
        		}
        	}
        	if(currentChoice==19)
        	{
        		int dis,choice;
        		int i,j,x,y;
        		x=e.getX();
        		y=e.getY();
        		if(e.getButton()==MouseEvent.BUTTON3)
        		{
        			for(j=0; j<index; j++)
            			itemList[j].ischoose=false;
        			repaint();
        		}
        		else
        		{
	        		dis=(itemList[0].x1-x)*(itemList[0].x1-x)+(itemList[0].y1-y)*(itemList[0].y1-y);
	        		choice=0;
	        		for(i=0; i<index; i++)
	        		{
	        			if((itemList[i].x1-x)*(itemList[i].x1-x)+(itemList[i].y1-y)*(itemList[i].y1-y)<dis)
	        			{
	        				dis=(itemList[i].x1-x)*(itemList[i].x1-x)+(itemList[i].y1-y)*(itemList[i].y1-y);
	        				choice=i;
	        			}
	        		}
	        		for(j=0; j<index; j++)
	        			itemList[j].ischoose=false;
	        		itemList[choice].ischoose=true;
	        		int forward=choice;
	        		int backward=choice;
	        		while((forward>=1)&&(itemList[forward-1].x2==itemList[forward].x1)&&(itemList[forward-1].y2==itemList[forward].y1))
	        		{
	        			itemList[forward-1].ischoose=true;
	        			forward--;
	        		}
	        		while((backward<=index-1)&&(itemList[backward+1].x1==itemList[backward].x2)&&(itemList[backward+1].y1==itemList[backward].y2))
	        		{
	        			itemList[backward+1].ischoose=true;
	        			backward++;
	        		}
	        		repaint();
        		}
        	}
        }
        public void mouseEntered(MouseEvent e) {
            statusBar.setText("     Mouse Entered @:[" + e.getX() +
                    ", " + e.getY() + "]");
        }
        public void mouseExited(MouseEvent e) {
            statusBar.setText("     Mouse Exited @:[" + e.getX() +
                    ", " + e.getY() + "]");
        }
    }
//����¼�mouseB��̳���MouseMotionAdapter�������������϶�������ƶ�ʱ����Ӧ����
    class mouseB extends MouseMotionAdapter {
        public void mouseDragged(MouseEvent e) {
            statusBar.setText("     Mouse Dragged @:[" + e.getX() +
                    ", " + e.getY() + "]");
            if((currentChoice!=10)&&(currentChoice!=11)&&(currentChoice!=19))
            {
            	if (currentChoice == 0 || currentChoice == 12) {
                itemList[index - 1].x1 = itemList[index].x2 = itemList[index].x1 = e.getX();
                itemList[index - 1].y1 = itemList[index].y2 = itemList[index].y1 = e.getY();
                index++;
                createNewItem();
            	} else {
            		itemList[index].x2 = e.getX();
            		itemList[index].y2 = e.getY();
            	}
            	repaint();
            }    
            if(currentChoice==19)
            {
            	int i;           	
            	for(i=0; i<index; i++)
            		if(itemList[i].ischoose)
            		{
            			if(itemList[i].type==13)
            			{
            				itemList[i].x1+=(e.getX()-cur_x);
            				itemList[i].y1+=(e.getY()-cur_y);            			
            			}
            			else
            			{
            				itemList[i].x1+=(e.getX()-cur_x);
            				itemList[i].y1+=(e.getY()-cur_y);
            				itemList[i].x2+=(e.getX()-cur_x);
            				itemList[i].y2+=(e.getY()-cur_y);  
            				itemList[i].init_x+=(e.getX()-cur_x);
            				itemList[i].init_y+=(e.getY()-cur_y);  
            			}
            		}
            	repaint();
            	cur_x=e.getX();
				cur_y=e.getY();
            }
        }
        public void mouseMoved(MouseEvent e) {
            statusBar.setText("     Mouse Moved @:[" + e.getX() +
                    ", " + e.getY() + "]");
        }
    }
//ѡ��������ʱ���õ����¼��������࣬���뵽�������ѡ�����
    private class checkBoxHandler implements ItemListener {
        public void itemStateChanged(ItemEvent e) {
            if (e.getSource() == bold) {
                if (e.getStateChange() == ItemEvent.SELECTED) {
                    f1 = Font.BOLD;
                } else {
                    f1 = Font.PLAIN;
                }   
                int i;
                for(i=0; i<index; i++)
                	if((itemList[i].ischoose)&&(itemList[i].type==13))
                	{
                		itemList[i].x2=f1;
                		break;
                	}
                repaint();
            }
            if (e.getSource() == italic) {
                if (e.getStateChange() == ItemEvent.SELECTED) {
                    f2 = Font.ITALIC;
                } else {
                    f2 = Font.PLAIN;
                }
                int i;
                for(i=0; i<index; i++)
                	if((itemList[i].ischoose)&&(itemList[i].type==13))
                	{
                		itemList[i].y2=f2;
                		break;
                	}
                repaint();
            }
        }
    }
//��ͼ����࣬������ͼ
    class DrawPanel extends JPanel {
        public DrawPanel() {
            setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
            setBackground(Color.white);
            addMouseListener(new mouseA());
            addMouseMotionListener(new mouseB());
        }
        @Override
        public void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2d = (Graphics2D) g;    //���廭��
            int j = 0;
            while (j <= index) {
                draw(g2d, itemList[j]);
                j++;
            }
        }
        void draw(Graphics2D g2d, drawings i) {
            i.draw(g2d);//�����ʴ��뵽���������У�������ɸ��ԵĻ�ͼ
        }
    }
//�½�һ����ͼ������Ԫ����ĳ����
    void createNewItem() {
        if (currentChoice == 13)//������Ӧ���α�����
        {
            drawingArea.setCursor(Cursor.getPredefinedCursor(Cursor.TEXT_CURSOR));
        } else {
            drawingArea.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
        }
        switch (currentChoice) {
            case 0:
                itemList[index] = new Pencil();
                break;
            case 1:
                itemList[index] = new Line();
                break;
            case 2:
                itemList[index] = new Rect();
                break;
            case 3:
                itemList[index] = new fillRect();
                break;
            case 4:
                itemList[index] = new Oval();
                break;
            case 5:
                itemList[index] = new fillOval();
                break;
            case 6:
                itemList[index] = new Circle();
                break;
            case 7:
                itemList[index] = new fillCircle();
                break;
            case 8:
                itemList[index] = new RoundRect();
                break;
            case 9:
                itemList[index] = new fillRoundRect();
                break;
            case 10:
            	itemList[index] = new Polyline();
                break;
            case 11:
            	itemList[index] = new Polygon();
                break;
            case 12:
                itemList[index] = new Rubber();
                break;
            case 13:
                itemList[index] = new Word();
                break;
            default:
            	itemList[index] = new drawings();
            	break;
        }        
        itemList[index].type = currentChoice;
        itemList[index].R = R;
        itemList[index].G = G;
        itemList[index].B = B;
        itemList[index].stroke = stroke;              
    }
//ѡ��ǰ��ɫ�����
    public void chooseColor() {
        color = JColorChooser.showDialog(Mini_CAD.this,
                "Choose a color", color);
        if(color!=null)
        {
        	R = color.getRed();
        	G = color.getGreen();
        	B = color.getBlue();
        }        
        int i;
        for(i=0; i<index; i++)
        	if(itemList[i].ischoose)
        	{
        		itemList[i].R=R;
        		itemList[i].G=G;
        		itemList[i].B=B;
        		itemList[i].ischoose=false;
        	}
        repaint();
        itemList[index].R = R;
        itemList[index].G = G;
        itemList[index].B = B;
    }
//ѡ��ǰ������ϸ�����
    public void setStroke() {
        String input;
        input = JOptionPane.showInputDialog(
                "Please input a float stroke value! ( >0 )");
        stroke = Float.parseFloat(input);
        int i;
        for(i=0; i<index; i++)
        	if((itemList[i].ischoose)&&(itemList[i].type!=13))
        	{
        		itemList[i].stroke=stroke;
        		itemList[i].ischoose=false;
        	}
        repaint();
        itemList[index].stroke = stroke;
    }
    public void setText() {
        String input;         
        int i;
        for(i=0; i<index; i++)
        	if((itemList[i].ischoose)&&(itemList[i].type==13))
        	{
        		input = JOptionPane.showInputDialog(
                        "Please input the text!");       
        		itemList[i].s1=input;
        		itemList[i].ischoose=false;
        		break;
        	}
        if(i>=index)
        	JOptionPane.showMessageDialog(null,
                    "Please select a valid text component!",
                    "Hint", JOptionPane.INFORMATION_MESSAGE);
        repaint();
    }
//����ͼ���ļ������
    public void saveFile() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int result = fileChooser.showSaveDialog(this);
        if (result == JFileChooser.CANCEL_OPTION) {
            return;
        }
        File fileName = fileChooser.getSelectedFile();
        fileName.canWrite();
        if (fileName == null || fileName.getName().equals("")) {
            JOptionPane.showMessageDialog(fileChooser, "Invalid File Name",
                    "Invalid File Name", JOptionPane.ERROR_MESSAGE);
        } else {
            try {
                fileName.delete();
                FileOutputStream fos = new FileOutputStream(fileName);
                output = new ObjectOutputStream(fos);
                //drawings record;
                output.writeInt(index);
                for (int i = 0; i < index; i++) {
                    drawings p = itemList[i];
                    output.writeObject(p);
                    output.flush();    //������ͼ����Ϣǿ��ת���ɸ������Ի��洢���ļ���
                }
                output.close();
                fos.close();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }
    }
//��һ��ͼ���ļ������
    public void loadFile() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.CANCEL_OPTION) {
            return;
        }
        File fileName = fileChooser.getSelectedFile();
        fileName.canRead();
        if (fileName == null || fileName.getName().equals("")) {
            JOptionPane.showMessageDialog(fileChooser, "Invalid File Name",
                    "Invalid File Name", JOptionPane.ERROR_MESSAGE);
        } else {
            try {
                FileInputStream fis = new FileInputStream(fileName);
                input = new ObjectInputStream(fis);
                drawings inputRecord;
                int countNumber = 0;
                countNumber = input.readInt();
                for (index = 0; index < countNumber; index++) {
                    inputRecord = (drawings) input.readObject();
                    itemList[index] = inputRecord;
                }
                createNewItem();
                input.close();
                repaint();
                currentChoice=19;
            } catch (EOFException endofFileException) {
                JOptionPane.showMessageDialog(this, "no more record in file",
                        "class not found", JOptionPane.ERROR_MESSAGE);
            } catch (ClassNotFoundException classNotFoundException) {
                JOptionPane.showMessageDialog(this, "Unable to Create Object",
                        "end of file", JOptionPane.ERROR_MESSAGE);
            } catch (IOException ioException) {
                JOptionPane.showMessageDialog(this, "error during read from file",
                        "read Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }
//�½�һ���ļ������
    public void newFile() {
        index = 0;
        currentChoice = 19;
        color = Color.black;
        stroke = 1.0f;
        createNewItem();
        repaint();//���й�ֵ����Ϊ��ʼ״̬�������ػ�
    }
//��������
    public static void main(String args[]) {
        Mini_CAD newPad = new Mini_CAD();
        newPad.setLocationRelativeTo(null);
        newPad.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
class drawings implements Serializable//���࣬����ͼ�ε�Ԫ���õ����л��ӿڣ�����ʱ����
{
    int x1, y1, x2, y2; //������������    
    int dx,dy;
    int R, G, B;        //����ɫ������
    int init_x,init_y;
    boolean end;
    boolean ischoose=false;
    float stroke;       //����������ϸ����
    int type;       
    String s1;
    void draw(Graphics2D g2d) {
    }
    ;//�����ͼ����
}
/*******************************************************************************
�����Ǹ��ֻ���ͼ�ε�Ԫ�����࣬���̳��Ը���drawings
 ********************************************************************************/
class Line extends drawings //ֱ����
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
    		g2d.setStroke(new BasicStroke(stroke,
    				BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
    		g2d.drawLine(x1, y1, x2, y2);
    		stroke=stroke/2;
    	}    		
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
    		g2d.setStroke(new BasicStroke(stroke,
    				BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
    		g2d.drawLine(x1, y1, x2, y2);
    	}
        
    }
}
class Rect extends drawings//������
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
    	}        
    }
}
class fillRect extends drawings//ʵ�ľ�����
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
    	}       
    }
}
class Oval extends drawings//��Բ��
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
    	}       
    }
}
class fillOval extends drawings//ʵ����Բ
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2));
    	}     
    }
}
class Circle extends drawings//Բ��
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;   		
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)));
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)));
    	}        
    }
}
class fillCircle extends drawings//ʵ��Բ
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)));
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillOval(Math.min(x1, x2), Math.min(y1, y2),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)),
                    Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)));
    	}       
    }
}
class RoundRect extends drawings//Բ�Ǿ�����
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawRoundRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2),
                    50, 35);
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.drawRoundRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2),
                    50, 35);
    	}      
    }
}
class fillRoundRect extends drawings//ʵ��Բ�Ǿ�����
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillRoundRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2),
                    50, 35);
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke));
            g2d.fillRoundRect(Math.min(x1, x2), Math.min(y1, y2),
                    Math.abs(x1 - x2), Math.abs(y1 - y2),
                    50, 35);
    	}        
    }
}
class Pencil extends drawings//��ʻ���
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	g2d.setPaint(new Color(R, G, B));
		g2d.setStroke(new BasicStroke(stroke,
				BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
		g2d.drawLine(x1, y1, x2, y2);
    }
}
class Polyline extends drawings//���������
{	
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke,
                    BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
            g2d.drawLine(x1, y1, x2, y2);
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke,
                    BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
            g2d.drawLine(x1, y1, x2, y2);
    	}        
    }
}
class Polygon extends drawings//�������
{	
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
    	if(ischoose)
    	{
    		stroke=stroke*2;
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setStroke(new BasicStroke(stroke,
                    BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
            g2d.drawLine(x1, y1, x2, y2);
            if(end)
            	g2d.drawLine(x2, y2, init_x, init_y);
            stroke=stroke/2;
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setStroke(new BasicStroke(stroke,
                    BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
            g2d.drawLine(x1, y1, x2, y2);
            if(end)
            	g2d.drawLine(x2, y2, init_x, init_y);
    	}
    }
}
class Rubber extends drawings//��Ƥ����
{
    void draw(Graphics2D g2d) {
    	this.dx=x2-x1;
    	this.dy=y2-y1;
        g2d.setPaint(new Color(255, 255, 255));
        g2d.setStroke(new BasicStroke(stroke + 4,
                BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL));
        g2d.drawLine(x1, y1, x2, y2);
    }
}
class Word extends drawings//����������
{
    void draw(Graphics2D g2d) {
    	if(ischoose)
    	{    		
    		g2d.setPaint(new Color(255, 0, 0));
            g2d.setFont(new Font(null, x2 + y2, ((int) stroke) * 18));
            if (s1 != null) {
                g2d.drawString(s1, x1, y1);
            }            
    	}
    	else
    	{
    		g2d.setPaint(new Color(R, G, B));
            g2d.setFont(new Font(null, x2 + y2, ((int) stroke) * 18));
            if (s1 != null) {
                g2d.drawString(s1, x1, y1);
            }
    	}
    }
}
