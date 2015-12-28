import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class FontChecker extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JLabel lblQuestion = new JLabel( "Is this a readable font?" );
	private JLabel lblDisplay = new JLabel( "0 1 2 3 4 5 6 7 8 9" );
	private JPanel pnlYesNo = new JPanel( new GridLayout( 1 , 2 ) );
	private JButton cmdYes = new JButton( "Yes" );
	private JButton cmdNo = new JButton( "No" );
	
	int fontIdx = 0;
	public FontChecker() throws Exception {
		add( this.lblQuestion , BorderLayout.NORTH );
		add( this.lblDisplay , BorderLayout.CENTER );
		this.lblDisplay.setFont( FontManager.fonts.get( 0 ).deriveFont( Font.PLAIN , 12.0f ) );
		
		cmdYes.addActionListener( new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				System.out.println( FontManager.fonts.get( fontIdx ).getFontName() );
				++fontIdx;
				if ( fontIdx >= FontManager.fonts.size() ) {
					System.exit( 0 );
				}
				lblDisplay.setFont( FontManager.fonts.get( fontIdx ).deriveFont( Font.PLAIN , 12.0f ) );
			}
			
		});
		
		cmdNo.addActionListener( new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				++fontIdx;
				if ( fontIdx >= FontManager.fonts.size() ) {
					System.exit( 0 );
				}
				lblDisplay.setFont( FontManager.fonts.get( fontIdx ).deriveFont( Font.PLAIN , 12.0f ) );
			}
			
		});
		
		this.pnlYesNo.add( cmdYes );
		this.pnlYesNo.add( cmdNo );
		add( this.pnlYesNo , BorderLayout.SOUTH );
		setSize( 200 , 200 );
		setVisible( true );
		setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
	}
	
	

}
