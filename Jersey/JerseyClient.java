package test;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.Entity;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Form;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.Test;

public class JerseyClient {
	public final static String get_text_path = "http://localhost:80/WebProject/test/get_text";
	public final static String post_form_path = "http://localhost:80/WebProject/test/post_form";
	public final static String post_json_path = "http://localhost:80/WebProject/test/post_json";
	public final static String get_json_path = "http://localhost:80/WebProject/test/get_json";
	public final static String put_json_path = "http://localhost:80/WebProject/test/put_json";
	public final static String put_text_path = "http://localhost:80/WebProject/test/put_text";
	public final static String get_xml_path = "http://localhost:80/WebProject/test/get_xml";
	public static Client client = ClientBuilder.newClient();
	
	public static void getXml() {
		WebTarget target = client.target(get_xml_path);
		Response response = target.request().get();
		try {
			if (response.getStatus() != 200) {
				System.out.println(response.getStatus());
				System.out.println("status error");
			}
			System.out.println(response.readEntity(String.class));
		} finally {
			response.close();
		}
	}
	
	public static void getText() {
		WebTarget target = client.target(get_text_path);
		Response response = target.request().get();
		try {
			if (response.getStatus() != 200) {
				System.out.println("status error");
			}
			System.out.println(response.readEntity(String.class));
		} finally {
			response.close();
		}
	}
	
	public static void getJson() {
		WebTarget target = client.target(get_json_path);
		Response response = target.request().get();
		try {
			if (response.getStatus() != 200) {
				System.out.println(response.getStatus());
				System.out.println("status error");
			}
			System.out.println(response.readEntity(String.class));
		} finally {
			response.close();
		}
	}
	
	public static void postJson() {
		WebTarget target = client.target(post_json_path);
		
		Student student = new Student(2, "post_json");
		
		Response response = target.request(MediaType.APPLICATION_JSON).post(Entity.entity(student, MediaType.APPLICATION_JSON));
		if (response.getStatus() != 200) {
			System.out.println(response.getStatus());
			System.out.println("status error");
		}
		System.out.println(response.readEntity(String.class));
		response.close();
	}
	
	public static void postForm() {
		WebTarget target = client.target(post_form_path);
		Form form = new Form();
		form.param("post", "form");
		Response response = target.request(MediaType.TEXT_PLAIN).post(Entity.entity(form, MediaType.TEXT_PLAIN));
		if (response.getStatus() != 200) {
			System.out.println(response.getStatus());
			System.out.println("status error");
		}
		System.out.println(response.readEntity(String.class));
		response.close();
	}
	

	
	public static void putJson() {
		WebTarget target = client.target(put_json_path);
		
		Student student = new Student(3, "put_json");
		
		Response response = target.request(MediaType.APPLICATION_JSON).put(Entity.entity(student, MediaType.APPLICATION_JSON));
		if (response.getStatus() != 200) {
			System.out.println(response.getStatus());
			System.out.println("status error");
		}
		System.out.println(response.readEntity(String.class));	
		response.close();
	}
	
	public static void putText() {
		WebTarget target = client.target(put_text_path);
		String str = new String("putText");
		
		Response response = target.request(MediaType.TEXT_PLAIN).put(Entity.entity(str, MediaType.TEXT_PLAIN));
		if (response.getStatus() != 200) {
			System.out.println(response.getStatus());
			System.out.println("status error");
		}
		System.out.println(response.readEntity(String.class));	
		response.close();
	}
	
	@Test
	public void test() {
		getXml();
		getText();
		getJson();
		postForm();
		postJson();
		putJson();
		putText();	
		client.close();
	}
}
