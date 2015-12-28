import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;

public class Main {

	public static final void main( String[] args ) throws Exception {
		FontManager.init();
		Generator g = new Generator();
		g.generate( 2 );
		BufferedImage output = g.generateN( 2 , 25 , 5 , 5 );
		ImageIO.write( output , "png" , new File( "test.png" ) );
	}
}
