import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.GraphicsEnvironment;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class FontManager {

	public static ArrayList< Font > fonts = new ArrayList< Font >();
	
	public static void init() throws IOException , FontFormatException {
	    ArrayList< String > allowedFonts = new ArrayList< String >();
	    
	    BufferedReader f = new BufferedReader( new FileReader( "fonts/allowed_fonts" ) );
	    String fontName = f.readLine();
	    while( fontName != null ) {
	    	allowedFonts.add( fontName );
	    	fontName = f.readLine();
	    }
	    
		GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
		
		//Add in any custom fonts to the allowed list
		//fonts.add(Font.createFont( Font.TRUETYPE_FONT , new File( "fonts/divisible-invisible-high.ttf" ) ).deriveFont( Font.BOLD , 12 ) );
		for ( Font font : fonts ) {
			ge.registerFont( font );
			allowedFonts.add( font.getFontName() );
		}
		
		fonts.clear();
		
		for ( Font font : ge.getAllFonts() ) {
			if ( allowedFonts.contains( font.getName() ) ) {
				fonts.add( font );
			}
		}
		
	}
	
	public static Font getRandomFont() {
		int randIdx = (int)(Math.random()*fonts.size());
		return fonts.get( randIdx ).deriveFont( Font.PLAIN , 12.0f );
	}
	
	public static Font getRandomFont( boolean allowBold , int sizeMin , int sizeMax ) {
		int randIdx = (int)(Math.random()*fonts.size());
		int randSize = sizeMin + (int)(Math.random()*(sizeMax-sizeMin));
		int randType = Font.PLAIN;
		if ( allowBold ) {
			if ( Math.random() > 0.5 ) {
				randType = Font.BOLD;
			}
		}
		return fonts.get( randIdx ).deriveFont( randType , randSize );
	}
}
