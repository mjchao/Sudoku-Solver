import java.awt.Color;
import java.awt.FontFormatException;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

/**
 * Generates 28x28 images of digits
 * @author mjchao
 *
 */
public class Generator extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private final DigitPanel pnlDigit = new DigitPanel();
	
	public Generator() throws Exception {
		
		setLayout( null );
		add( this.pnlDigit );
		int pnlX = 10;
		int pnlY = 10;
		int pnlWidth = 28;
		int pnlHeight = 28;
		this.pnlDigit.setLocation( pnlX , pnlY );
		this.pnlDigit.setBounds( pnlX , pnlY , pnlX+pnlWidth , pnlY+pnlHeight );
		
		setSize( 100 , 100 );
		setVisible( true );
		setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
	}
	
	public Color getRandomColor() {
		return new Color( (int)(Math.random()*255) , (int)(Math.random()*255) , (int)(Math.random()*255) );
	}
	
	public boolean tooSimilar( Color color1 , Color color2 ) {
		
		//convert to YUC space and apply a euclidean distance check
		int r1 = color1.getRed() , g1 = color1.getGreen() , b1 = color1.getBlue();
		double y1 = .299*r1 + .587*g1 + .114*b1;
		double u1 = -.14713*r1 + -.28886*g1 + .436*b1; 
		double c1 = .615*r1 + -.51499*g1 + -.10001*b1;
		
		int r2 = color2.getRed() , g2 = color2.getGreen() , b2 = color2.getBlue();
		double y2 = .299*r2 + .587*g2 + .114*b2;
		double u2 = -.14713*r2 + -.28886*g2 + .436*b2; 
		double c2 = .615*r2 + -.51499*g2 + -.10001*b2;
		
		double dy = y1 - y2;
		double du = u1 - u2;
		double dc = c1 - c2;
		
		int dr = Math.abs( r1 - r2 );
		int dg = Math.abs( g1 - g2 );
		int db = Math.abs( b1 - b2 );
		return dr <= 30 || dg <= 30 || db <= 30 || (du*du + dc*dc <= 100*100) ;
	}
	
	public void generate( int digit) {
		this.pnlDigit.img = new BufferedImage( 28 , 28 , BufferedImage.TYPE_INT_RGB );
		Color backgroundColor = getRandomColor();
		Color fontColor = getRandomColor();
		while( tooSimilar( backgroundColor , fontColor ) ) {
			backgroundColor = getRandomColor();
			fontColor = getRandomColor();
		}
		this.pnlDigit.setBackgroundColor( backgroundColor );
		this.pnlDigit.setFontColor( fontColor );
		this.pnlDigit.setFont( FontManager.getRandomFont( true , 12 , 24 ) );
		this.pnlDigit.setDigit( digit );
		
		try {
			SwingUtilities.invokeAndWait( new Runnable() {

				@Override
				public void run() {
					pnlDigit.repaint();
				}
			} 
			);
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Generate n images of a digit and stitch them together
	 * into an image of row-by-cols tesselation
	 * 
	 * @param digit
	 * @param n
	 * @param rows
	 * @param cols
	 */
	public BufferedImage generateN( int digit , int n , int rows , int cols , String saveDir ) {
		BufferedImage rtn = new BufferedImage( cols*28 , rows*28 , BufferedImage.TYPE_BYTE_GRAY );
		Graphics2D g = rtn.createGraphics();
		for ( int i=0 ; i<n ; ++i ) {
			generate( digit );
			BufferedImage toStitch = this.pnlDigit.img;
			int currRow = i / cols;
			int currCol = i % cols;
			try {
				SwingUtilities.invokeAndWait( new Runnable() {
					
					@Override
					public void run() {
						g.drawImage( toStitch , currCol*28 , currRow*28 , null );
					}
				});
			} catch (InvocationTargetException e1) {
				e1.printStackTrace();
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}
			
			try {
				saveTrainingImageData( digit , i , toStitch , saveDir );
			}
			catch ( IOException e ) {
				System.out.println( "Failed to save training data." );
			}
		}
		return rtn;
	}
	
	public void saveTrainingImageData( int digit , int imgIdx , BufferedImage toSave , String saveDir ) throws IOException {
		byte[] data = new byte[ 28*28 ];
		for ( int y=0 ; y<28 ; ++y ) {
			for ( int x=0 ; x<28 ; ++x ) {
				int color = toSave.getRGB( x , y );
				int red = (color & 0x00ff0000) >> 16;
				int green = (color & 0x0000ff00) >> 8;
				int blue = (color & 0x000000ff) >> 0;
				int average = (red + green + blue)/3;
				data[ y*28+x ] = (byte)(average);
			}
		}
		String filename = saveDir + "/" + digit + "/" + digit;
		FileOutputStream out = new FileOutputStream( filename , true );
		try {
			out.write( data );
		}
		catch ( IOException e ) {
			//ignore
		}
		out.close();
	}
	

	private class DigitPanel extends JPanel {

		private String toDisplay = "0";
		private Color fontColor = Color.BLACK;
		private Color backgroundColor = Color.WHITE;
		private BufferedImage img = new BufferedImage( 28 , 28 , BufferedImage.TYPE_INT_RGB );
		private Random rand = new Random();
		
		public DigitPanel() throws IOException , FontFormatException {
			this.setOpaque( true );
		}
		
		private void updateText() {
			repaint();
		}
		
		public void setDigit( int digit ) {
			this.toDisplay = String.valueOf( digit );
			updateText();
		}
		
		public void setFontColor( Color c ) {
			this.fontColor = c;
			repaint();
		}
		
		public void setBackgroundColor( Color c ) {
			this.backgroundColor = c;
			this.setBackground( c );
			repaint();
		}
		
		/**
		 * Draws the given string in the middle of this panel
		 * with some random offsets.
		 * 
		 * @param s
		 * @param g
		 */
		private void drawString( String s , Graphics2D g ) {
			FontMetrics metrics = this.getFontMetrics( this.getFont() );
			int stringWidth = metrics.stringWidth( s );
			int stringHeight = metrics.getHeight();
			
			double x = (this.img.getWidth() - stringWidth)/2.0;
			double y = (this.img.getHeight() - stringHeight)/2.0 + metrics.getAscent();
			g.drawString( s , (float)x , (float)y );
		}
		
		private void addNoiseToImage() {
			int noiseMagnitude = 1;
			for ( int x=0 ; x<this.img.getWidth() ; ++x ) {
				for ( int y=0 ; y<this.img.getHeight() ; ++y ) {
					int colorBefore = this.img.getRGB( x , y );
					int dR = (int)(noiseMagnitude * rand.nextGaussian());
					int dG = (int)(noiseMagnitude * rand.nextGaussian());
					int dB = (int)(noiseMagnitude * rand.nextGaussian());
					int colorAfter = colorBefore + (dB << 0) + (dG << 8) + (dR << 16);
					this.img.setRGB( x , y , colorAfter );
				}
			}
		}
		
		@Override
		public void paintComponent( Graphics graphics ) {
			Graphics2D g = this.img.createGraphics();
			g.setPaint( this.backgroundColor );
			g.fillRect( 0 , 0 , this.img.getWidth() , this.img.getHeight() );
			
			g.setPaint( this.fontColor );
			g.setFont( this.getFont() );
			
			drawString( this.toDisplay , g );
			addNoiseToImage();
			
			g = (Graphics2D) graphics;
			g.drawImage( this.img , 0 , 0 , null );
			
		}
	}
}
