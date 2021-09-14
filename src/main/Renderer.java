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
    public static final Map<Point, Agent> ownerMap = new HashMap<>();

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

            float deltaX = (float) (Math.sin(angle) * Agent.SPEED);
            float deltaY = (float) -(Math.cos(angle) * Agent.SPEED);

            float newX = agent.predictX(deltaX);
            float newY = agent.predictY(deltaY);

            if (newX < 0 || newX > Main.WIDTH-10 || newY < 0 || newY > Main.HEIGHT-10) {
                newX = Math.min(Main.WIDTH-10.0f, Math.max(0, newX));
                newY = Math.min(Main.HEIGHT-10.0f, Math.max(0, newY));

                //agent.setAngle(agent.getAngle() + Math.toRadians(45));
                agent.setAngle(Agent.generateRandomAngle());
            }

			//agent.setAngle(agent.getAngle() + Math.toRadians(5));

            agent.setLocation(newX, newY);

            double rightAngle = angle + Agent.SENSOR_ANGLE;
            double leftAngle = angle - Agent.SENSOR_ANGLE;

            Point front = new Vector2(agent.predictX((float) (Math.sin(angle) * Agent.SENSOR_DISTANCE)),
										agent.predictY((float) -(Math.cos(angle) * Agent.SENSOR_DISTANCE))).asPoint();
										
            Point right = new Vector2(agent.predictX((float) (Math.sin(leftAngle) * Agent.SENSOR_DISTANCE)),
										agent.predictY((float) -(Math.cos(leftAngle) * Agent.SENSOR_DISTANCE))).asPoint();

            Point left = new Vector2(agent.predictX((float) (Math.sin(rightAngle) * Agent.SENSOR_DISTANCE)),
									agent.predictY((float) -(Math.cos(rightAngle) * Agent.SENSOR_DISTANCE))).asPoint();

            
            int frontWeight = pixelMap.containsKey(front) ? pixelMap.get(front) : 0;
            int leftWeight = pixelMap.containsKey(left) ? pixelMap.get(left) : 0;
            int rightWeight = pixelMap.containsKey(right) ? pixelMap.get(right) : 0;

            if (frontWeight != 0) {
                frontWeight *= ownerMap.get(front) == agent ? 0 : 1;
            }
            
            if (leftWeight != 0) {
                leftWeight *= ownerMap.get(left) == agent ? 0 : 1;
            }

            if (rightWeight != 0) {
                rightWeight *= ownerMap.get(right) == agent ? 0 : 1;
            }

            double randomSteer = Agent.generateRandomAngle();

            if (frontWeight > leftWeight && frontWeight > rightWeight) {
                double newAngle = Math.atan2(front.y - newY, front.x - newX);
				newAngle += Math.PI/2;
				agent.setAngle(newAngle);
            } else if (frontWeight < leftWeight && frontWeight < rightWeight) {
                agent.setAngle(agent.getAngle() + randomSteer/10);
            } else if (rightWeight < leftWeight) {
                double newAngle = Math.atan2(left.y - newY, left.x - newX);
				newAngle += Math.PI/2;
				agent.setAngle(newAngle);
            } else if (rightWeight > leftWeight) {
                double newAngle = Math.atan2(right.y - newY, right.x - newX);
				newAngle += Math.PI/2;
				agent.setAngle(newAngle);
            }

            List<Trail> trailMap = agent.getTrailMap();
            List<Trail> deadTrails = new ArrayList<>();

            for (Trail trail : trailMap) {
                trail.update();

                if (trail.getLifeSpan() < 0) {
                    deadTrails.add(trail);
                } else {
                    pixelMap.put(trail.getLocation().asPoint(), trail.getRawColor());
                    ownerMap.put(trail.getLocation().asPoint(), agent);
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
