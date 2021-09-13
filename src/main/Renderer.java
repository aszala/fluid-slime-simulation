package main;

import javax.swing.JPanel;
import objects.Agent;
import objects.Trail;
import util.Vector2;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Ellipse2D;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

public class Renderer extends JPanel {

    private static List<Agent> agents = new ArrayList<>();
    private static List<Agent> agentsQueue = new ArrayList<>();

    public static final Map<Point, Integer> pixelMap = new HashMap<>();

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


            double angle = agent.getAngle();
            
            double leftAngle = angle - Agent.SENSOR_ANGLE;
            double rightAngle = angle + Agent.SENSOR_ANGLE;

            float deltaX = (float) (Math.cos(angle) * Agent.SPEED);
            float deltaY = (float) -(Math.sin(angle) * Agent.SPEED);

            Point front = new Vector2(agent.predictX((float) (Math.cos(angle) * Agent.SPEED)), agent.predictY((float) (Math.sin(angle) * Agent.SPEED))).asPoint();
            Point left = new Vector2(agent.predictX((float) (Math.cos(leftAngle) * Agent.SPEED)), agent.predictY((float) (Math.sin(leftAngle) * Agent.SPEED))).asPoint();
            Point right = new Vector2(agent.predictX((float) (Math.cos(rightAngle) * Agent.SPEED)), agent.predictY((float) (Math.sin(rightAngle) * Agent.SPEED))).asPoint();

            g2d.setColor(Color.red);
            g2d.fillOval(front.x, front.y, 2, 2);
            g2d.setColor(Color.green);
            g2d.fillOval(left.x, left.y, 2, 2);
            g2d.setColor(Color.yellow);
            g2d.fillOval(right.x, right.y, 2, 2);

            List<Trail> trailMap = agent.getTrailMap();

            try {
                for (Trail trail : trailMap) {
                    g2d.setColor(trail.getColor());
                    g2d.draw(new Ellipse2D.Float(trail.getX(), trail.getY(), 2.0f, 2.0f));

                    //Todo implement diffusion
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

            double leftAngle = angle - Agent.SENSOR_ANGLE;
            double rightAngle = angle + Agent.SENSOR_ANGLE;

            Point front = new Vector2(agent.predictX((float) (Math.cos(angle) * Agent.SPEED)), agent.predictY((float) (Math.sin(angle) * Agent.SPEED))).asPoint();
            Point left = new Vector2(agent.predictX((float) (Math.cos(leftAngle) * Agent.SPEED)), agent.predictY((float) (Math.sin(leftAngle) * Agent.SPEED))).asPoint();
            Point right = new Vector2(agent.predictX((float) (Math.cos(rightAngle) * Agent.SPEED)), agent.predictY((float) (Math.sin(rightAngle) * Agent.SPEED))).asPoint();

            int frontWeight = pixelMap.containsKey(front) ? pixelMap.get(front) : 0;
            int leftWeight = pixelMap.containsKey(left) ? pixelMap.get(left) : 0;
            int rightWeight = pixelMap.containsKey(right) ? pixelMap.get(right) : 0;

            double randomSteer = Agent.generateRandomAngle();

            if (frontWeight > leftWeight && frontWeight > rightWeight) {
                agent.setAngle(agent.getAngle() + 0.0);
            } else if (frontWeight < leftWeight && frontWeight < rightWeight) {
                agent.setAngle(agent.getAngle() - (randomSteer - 0.5) * 2 * 0.5);
            } else if (rightWeight > leftWeight) {
                agent.setAngle(agent.getAngle() + randomSteer * 0.5);
            } else if (rightWeight < leftWeight) {
                agent.setAngle(agent.getAngle() - randomSteer * 0.5);
            }

            List<Trail> trailMap = agent.getTrailMap();
            List<Trail> deadTrails = new ArrayList<>();

            for (Trail trail : trailMap) {
                trail.update();

                if (trail.getLifeSpan() < 0) {
                    deadTrails.add(trail);
                } else {
                    pixelMap.put(trail.getLocation().asPoint(), trail.getRawColor());
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