import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;

public class Main {

	public static final void main( String[] args ) throws Exception {
		FontManager.init();
		Generator g = new Generator();
		g.generate( 2 );
		for ( int digit = 0 ; digit<=9 ; ++digit ) {
			BufferedImage output = g.generateN( digit , 2500 , 50 , 50 );
			ImageIO.write( output , "png" , new File( digit + "-tiles.png" ) );
		}
	}
}
