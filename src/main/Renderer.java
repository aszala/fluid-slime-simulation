package main;

import javax.swing.JPanel;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

public class Renderer extends JPanel {

    public Renderer() {
        setBackground(Color.black);
        setLocation(0, 0);
        setSize(Main.WIDTH, Main.HEIGHT);
    }

    @Override
    public void paintComponent(Graphics g) {
        Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(Color.green);
		
		g2d.fillRect(0, 0, Main.WIDTH, Main.HEIGHT);
    }

    public void update() {
        
    }

}
