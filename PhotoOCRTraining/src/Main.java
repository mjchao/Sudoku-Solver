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
		
		//set to "test" to generate test data
		//set to "train" to generate train data
		//set to "cv-n" to generate n-th cross-validation set 
		String saveDir = "test";
		for ( int digit = 0 ; digit<=9 ; ++digit ) {
			//clear the data file for the given digit, so that we don't keep appending
			//too much data
			PrintWriter out = new PrintWriter( new BufferedWriter( new FileWriter( saveDir + "/" + digit + "/" + digit ) ) );
			out.close();
			BufferedImage output = g.generateN( digit , 2500 , 50 , 50 , saveDir );
			ImageIO.write( output , "png" , new File( saveDir + "/" + digit + "-tiles.png" ) );
		}
	}
}
