package util;

import java.io.IOException;
import org.markdown4j.*;

/**
 * markdown�﷨������
 * ������markdown4j��Դ��
 * @version 1.0
 *
 */

public class Parser {
	private String mHTML = null;
	private String mMarkdownText = null;
	private Markdown4jProcessor mParser = new Markdown4jProcessor();
	
	public static void main(String[] args) {
		Parser parser = new Parser();
		String m = "# ��˷���\n"
				+ "\n"
				+ ""
				+ "[google](http://www.google.com)";
		
		try {
			System.out.println(parser.parseMarkdownToHTML(m));
		} catch(IOException e) {
			System.out.println("wrong");
		}
	}
	
	public Parser() {}
	
	public String parseMarkdownToHTML(String markdown) throws IOException {
		mMarkdownText = markdown;
		String tmp = mParser.process(mMarkdownText);
		tmp = "<html>\n"
				+ "<head>\n"
				+ "<style>\n"
				+ "</style>\n"
				+ "</head>\n"
				+ "<body>\n"
				+ tmp
				+ "</body>\n"
				+ "</html>\n";
		mHTML = tmp;
		return mHTML;
	}
	
	public String getHTML() {
		return mHTML;
	}
}

