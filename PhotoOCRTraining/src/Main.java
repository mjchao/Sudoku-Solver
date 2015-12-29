import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

import javax.imageio.ImageIO;

public class Main {

	public static final void main( String[] args ) throws Exception {
		FontManager.init();
		Generator g = new Generator();
		for ( int digit = 0 ; digit<=9 ; ++digit ) {
			//clear the data file for the given digit, so that we don't keep appending
			//too much data
			PrintWriter out = new PrintWriter( new BufferedWriter( new FileWriter( "train/" + digit + "/" + digit ) ) );
			out.close();
			BufferedImage output = g.generateN( digit , 2500 , 50 , 50 );
			ImageIO.write( output , "png" , new File( digit + "-tiles.png" ) );
		}
	}
}
