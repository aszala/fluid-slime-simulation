package main;

import javax.swing.JPanel;
import objects.Agent;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;
import java.util.ArrayList;
import java.util.List;

public class Renderer extends JPanel {

    private static List<Agent> agents = new ArrayList<>();
    private static List<Agent> agentsQueue = new ArrayList<>();

    public Renderer() {
        setBackground(Color.black);
        setLocation(0, 0);
        setSize(Main.WIDTH, Main.HEIGHT);
    }

    @Override
    public void paintComponent(Graphics g) {
        Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(Color.black);
		g2d.fillRect(0, 0, Main.WIDTH, Main.HEIGHT);

        for (Agent agent : agents) {
            g2d.setColor(Color.white);
            g2d.draw(new Ellipse2D.Float(agent.getX(), agent.getY(), 2.0f, 2.0f));
        }
		
    }

    public void update() {
        for (Agent agent : agentsQueue) {
            agents.add(agent);
        }
        agentsQueue.clear();

        for (Agent agent : agents) {
            double angle = agent.getAngle();

            float deltaX = (float) (Math.cos(angle) * Agent.SPEED);
            float deltaY = (float) -(Math.sin(angle) * Agent.SPEED);

            agent.move(deltaX, deltaY);
        }
    }

    public static void spawnNewAgent(int x, int y) {
        agentsQueue.add(new Agent(x, y));
    }

}
