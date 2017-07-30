package test;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;

@XmlRootElement

public class Student {
		private int id;
		private String name;
		
//		@JsonCreator
//		public Student(@JsonProperty("id")   int id, 
//					   @JsonProperty("name") String name) {
//			this.id = id;
//			this.name = name;
//		}
		public Student() {
			
		}
		public Student(int id, String name) {
			this.id = id;
			this.name = name;
		}
		
		@XmlAttribute
		public int getId() {
			return this.id;
		}
		
		@XmlElement
		public String getName() {
			return this.name;
		}
		public void setId(int id) {
			this.id = id;
		}
		
		public void setName(String name) {
			this.name = name;
		}
		@Override
		public String toString() {
			StringBuilder stringBuilder = new StringBuilder();
			stringBuilder.append("Name: "+this.name+"\t"+ "Id: "+this.id);
			return stringBuilder.toString();
		}
}
