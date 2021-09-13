package main;

import javax.swing.JPanel;
import objects.Agent;
import objects.Trail;

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
            g2d.setColor(agent.getColor());
            g2d.draw(new Ellipse2D.Float(agent.getX(), agent.getY(), 2.0f, 2.0f));

            List<Trail> trailMap = agent.getTrailMap();

            try {
                for (Trail trail : trailMap) {
                    g2d.setColor(trail.getColor());
                    g2d.draw(new Ellipse2D.Float(trail.getX(), trail.getY(), 2.0f, 2.0f));
                }
            } catch (Exception e) {
                
            }
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

            float newX = agent.predictX(deltaX);
            float newY = agent.predictY(deltaY);

            if (newX < 0 || newX >= Main.WIDTH || newY < 0 || newY >= Main.HEIGHT) {
                newX = Math.min(Main.WIDTH-0.01f, Math.max(0, newX));
                newY = Math.min(Main.HEIGHT-0.01f, Math.max(0, newY));

                agent.setAngle(agent.getAngle() + Math.toRadians(45));
                //agent.setAngle(Agent.generateRandomAngle());
            }

            agent.setLocation(newX, newY);
            List<Trail> trailMap = agent.getTrailMap();
            List<Trail> deadTrails = new ArrayList<>();

            for (Trail trail : trailMap) {
                trail.update();

                if (trail.getLifeSpan() <= 0) {
                    deadTrails.add(trail);
                }
            }

            for (Trail t : deadTrails) {
                trailMap.remove(t);
            }
        }
    }

    public static void spawnNewAgent(int x, int y) {
        agentsQueue.add(new Agent(x, y));
    }

}
