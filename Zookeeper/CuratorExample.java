package com.curator.test;

import java.util.ArrayList;
import java.util.List;

import org.apache.curator.RetryPolicy;
import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.CuratorFrameworkFactory;
import org.apache.curator.framework.api.BackgroundCallback;
import org.apache.curator.framework.api.CuratorEvent;
import org.apache.curator.framework.api.CuratorListener;
import org.apache.curator.framework.recipes.cache.NodeCache;
import org.apache.curator.framework.recipes.cache.NodeCacheListener;
import org.apache.curator.framework.recipes.cache.PathChildrenCache;
import org.apache.curator.framework.recipes.cache.PathChildrenCacheEvent;
import org.apache.curator.framework.recipes.cache.PathChildrenCacheListener;
import org.apache.curator.retry.ExponentialBackoffRetry;
import org.apache.curator.utils.ZKPaths;
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.Watcher;

public class CuratorExample {
	public static final String CONNECTION_STRING = "101.201.118.55:2181";
	public static final String TEST_PATH = "/tests";
	public static final String CHILDREN_PATH = "/tests/children";
	
	public static CuratorFramework getClient() {
        RetryPolicy retryPolicy = new ExponentialBackoffRetry(1000, 3);
        CuratorFramework client = CuratorFrameworkFactory.builder()
                .connectString(CONNECTION_STRING)
                .retryPolicy(retryPolicy)
                .sessionTimeoutMs(100)
                .connectionTimeoutMs(100)
                .build();
        client.start();
        return client;
	}
	
	public static void create(CuratorFramework client, String path, byte[] data) throws Exception {
		// this will create the path with the data
		client.create().forPath(path, data);
	}
	
	public static void createEphemeral(CuratorFramework client, String path, byte[] data) throws Exception {
		//this will create the given EPHEMERAL ZNode with the given data
		client.create().withMode(CreateMode.EPHEMERAL).forPath(path, data);
	}
	
	public static String createEphemeralSequential(CuratorFramework client, String path, byte[] data) throws Exception {
		// this will create the given EphemeralSequential ZNode with the given data
		// data using Curator protection
		String result;
		result = client.create().withProtection().withMode(CreateMode.EPHEMERAL_SEQUENTIAL).forPath(path, data);
		return result;
	}
	
	public static String getData(CuratorFramework client, String path) throws Exception {
		//get data with string
		return new String(client.getData().forPath(path));
	}
	
	
	public static void setData(CuratorFramework client, String path, byte[] data) throws Exception {
		client.setData().forPath(path, data);
	}
	
	public static void setDataAsync(CuratorFramework client, String path, byte[] data) throws Exception {
		// set data for the given node asynchronously
		CuratorListener listener = new CuratorListener() {
			
			public void eventReceived(CuratorFramework client, CuratorEvent event)
					throws Exception {
				//do something
			}
		};
		client.getCuratorListenable().addListener(listener);
		client.setData().inBackground().forPath(path, data);
	}
	
	public static void setDataAsyncCallback	(CuratorFramework client, BackgroundCallback callback, String path, byte[] data) throws Exception {
		//  this is another method of getting notification of an async completion
		client.setData().inBackground(callback).forPath(path, data);
	}
	
	public static void delete(CuratorFramework client, String path) throws Exception {
		client.delete().forPath(path);
	}
	
	public static void guaranteedDelete(CuratorFramework client, String path) throws Exception {
		// delete the given node and guarantee that it completes
		client.delete().guaranteed().forPath(path);
	}
	
	public static List<String> watchedGetChildren(CuratorFramework client, String path) throws Exception {
		/**
		 * Get children and set a watcher on the node. The watcher notification
		 * will come through the CuratorListener (see setDataAsync() above).
		 */
		return client.getChildren().watched().forPath(path);
	}
	public static List<String> watchedGetChildren(CuratorFramework client, String path, Watcher watcher) throws Exception {
		/**
		 * Get children and set the given watcher on the node.
		 */
		return client.getChildren().usingWatcher(watcher).forPath(path);
	}
	
	public static PathChildrenCache ChildrenMonitor(CuratorFramework client, String path) throws Exception {
		
		//this is children monitor
		PathChildrenCache  cache = new PathChildrenCache(client, path, true);
		PathChildrenCacheListener listener = new PathChildrenCacheListener() {
            public void childEvent(CuratorFramework client, PathChildrenCacheEvent event) throws Exception {
                switch (event.getType()) {
                case CHILD_ADDED:
                    System.out.println("Event : CHILD_ADDED   " +  ZKPaths.getNodeFromPath(event.getData().getPath()));
                    break;
                case CHILD_UPDATED:
                    System.out.println("Event : CHILD_UPDATED " + ZKPaths.getNodeFromPath(event.getData().getPath()));
                    break;
                case CHILD_REMOVED:
                    System.out.println("Event : CHILD_REMOVED " + ZKPaths.getNodeFromPath(event.getData().getPath()));
                    break;
                default:
                    break;
                }
            }
        };
        cache.getListenable().addListener(listener);
        return cache;
	}
	
	public static NodeCache nodeListener(CuratorFramework client, String path) {
		
			//node monitor
	        final NodeCache cache = new NodeCache(client, path);
	        cache.getListenable().addListener(new NodeCacheListener() {
	            public void nodeChanged() throws Exception {
	                System.out.println("NodeCache changed, data is: " + new String(cache.getCurrentData().getData()));
	            }
	        });
	        return cache;
	    }
	
	public static void myprint(String data, String path) {
		System.out.println("path: "+ path + "\t" + "data: "+ data);
	}
	
	public static void main(String args[]) throws Exception {
		CuratorFramework client = getClient();
		create(client, TEST_PATH, "createSimple".getBytes());
		myprint(getData(client, TEST_PATH), TEST_PATH);
		delete(client, TEST_PATH);
		
		createEphemeral(client, TEST_PATH, "createEphemeral".getBytes());
		myprint(getData(client, TEST_PATH), TEST_PATH);
//		delete(client, TEST_PATH);
		
		createEphemeralSequential(client, TEST_PATH, "createEphemeralSequential".getBytes());
		myprint(getData(client, TEST_PATH), TEST_PATH);
		delete(client, TEST_PATH);
		
		create(client, TEST_PATH, "createSimple".getBytes());
		setData(client, TEST_PATH, "setdataSimple".getBytes());
		setData(client, TEST_PATH, "setdataAsync".getBytes());
		myprint(getData(client, TEST_PATH), TEST_PATH);
		
		create(client, CHILDREN_PATH, "createChildren".getBytes());
		List<String> list = new ArrayList<String>();
		list = watchedGetChildren(client, TEST_PATH);
		
		NodeCache nodeCache = nodeListener(client, CHILDREN_PATH);
		nodeCache.start(true);
		
		setData(client, CHILDREN_PATH, "setdataForWater".getBytes());
		for(String temp : list) {
			myprint(getData(client, TEST_PATH+"/"+temp), CHILDREN_PATH);
		}
		// sleep 3 s for zookeeper to get node change
		Thread.sleep(3000);
		delete(client, CHILDREN_PATH);
		delete(client, TEST_PATH);
		
	}
	
	
}
