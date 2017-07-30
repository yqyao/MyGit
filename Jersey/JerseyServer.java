package test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Form;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.MultivaluedMap;

@Path("/")
public class JerseyServer {
	  private static Map<Integer, Student> students = new HashMap<Integer, Student>();
	
	@GET
	@Path("/get_xml")
	@Produces(MediaType.APPLICATION_XML)
    public Student getXml() {
        // fetch all notifications
//        List<Student> students = new ArrayList<Student>();
//        students.add(new Student(1, "New user created"));
//        students.add(new Student(2, "New order created"));
		Student student = new Student();
		student.setId(1);
		student.setName("hello");
        return student;
    }	  
	  
    @GET
    @Path("/get_text")
    @Produces(MediaType.TEXT_PLAIN)
    public String getText() {
         return "get text test";
    }
    
    @GET
    @Path("/get_json")
    @Produces(MediaType.APPLICATION_JSON)
    public String getJson() {
    	Student student = new Student(1, "get_json");
    	return student.toString();
    }
    
    
    @POST
    @Path("/post_form")
    @Produces({MediaType.TEXT_PLAIN})
    public String postForm(Form form){  
    	MultivaluedMap<String, String> map = form.asMap();
    	System.out.println("post form: "+ map.get("post"));  
        return map.toString();  
    }  
    
    @POST
    @Path("/post_json")
    @Consumes("application/json")
    @Produces("application/json")
    public String postJson(Student student) {
    	
    	students.put(student.getId(), student);
    	System.out.print(student.toString());
    	return student.toString();
    }
    
    @PUT
    @Path("/put_json")
    @Consumes("application/json")
    @Produces("application/json")
    public String putJson(Student student) {
    	System.out.println(student.toString());
    	return student.toString();
    }
    @PUT
    @Path("put_text")
    @Produces(MediaType.TEXT_PLAIN)
    public String putText(String str) {
    	System.out.println("put text: "+ str);
    	return str;
    }
    
  
}


