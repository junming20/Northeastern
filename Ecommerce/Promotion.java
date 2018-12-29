
class Promotion {
	private String type;
	private String description;
	
	public Promotion(String type, String description) {
		// TODO Auto-generated constructor stub
		this.type = type;
		this.description = description;
	}
	
	public String getDescription() {
		// TODO Auto-generated method stub
		return description;
	}
	
	public String getType() {
		return type;
	}
	
	public void setType(String type) {
		this.type = type;
	}
	
	public void setDescription (String description) {
		this.description = description;
	}
}

