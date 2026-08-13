public class Contato {
    protected String nome;
    protected String endereco;
    protected String email;
    protected int id;

    public Contato(String nome, int id, String endereco, String email) {
        this.nome = nome;
        this.id = id;
        this.endereco = endereco;
        this.email = email;
    }

    public String getNome() {return this.nome;}
    public int getId() {return this.id;}
    public String getEndereco() {return this.endereco;}
    public String getEmail() {return this.email;}

    public void setNome(String nome) {this.nome = nome;}
    public void setEndereco(String endereco) {this.endereco = endereco;}
    public void setEmail(String email) {this.email = email;}
    public void setId(int id) {this.id = id;}

    @Override
    public String toString() {
        return "Nome: " + nome + "\n" + "Id: " + id + "\n";
    }
}
