import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileOutputStream;

import javax.imageio.ImageIO;

public class Main {

	public static final void main( String[] args ) throws Exception {
		FontManager.init();
		Generator g = new Generator();
		
		//set to "test" to generate test data
		//set to "train" to generate train data
		//set to "cv-n" to generate n-th cross-validation set 
		String saveDir = "test";
		
		//set to the number of training samples for each digit
		//make sure that rows*cols = numToGenerate or else
		//this will not work
		int numToGenerate = 2500;
		int rows = 50;
		int cols = 50;
		
		for ( int digit = 0 ; digit<=9 ; ++digit ) {
			
			//clear the data file for the given digit, so that we don't keep appending
			//too much data
			FileOutputStream out = new FileOutputStream( saveDir + "/" + digit + "/" + digit );
			
			//output the number of samples in Big Endian format to be
			//consistent with the MNIST training set
			byte[] sizeBytes = new byte[ 4 ];
			sizeBytes[ 3 ] = (byte)( numToGenerate >> 24 );
			sizeBytes[ 2 ] = (byte)( numToGenerate >> 16 );
			sizeBytes[ 1 ] = (byte)( numToGenerate >> 8 );
			sizeBytes[ 0 ] = (byte)( numToGenerate >> 0 );
			out.write( sizeBytes );
			out.close();
			BufferedImage output = g.generateN( digit , numToGenerate , rows , cols , saveDir );
			ImageIO.write( output , "png" , new File( saveDir + "/" + digit + "-tiles.png" ) );
		}
	}
}
